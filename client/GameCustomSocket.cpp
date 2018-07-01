#include "GameCustomSocket.h"
/*
这里用收发缓存的原因是处理socket的粘包问题和减少socket发送
*/


CGameSocket::CGameSocket()
{
	// 数据结构初始化  
	memset(m_bufOutput, 0, sizeof(m_bufOutput));
	memset(m_bufInput, 0, sizeof(m_bufInput));
}

void CGameSocket::closeSocket()
{
	// socket销毁
	closesocket(m_sockClient);
	WSACleanup();
}

bool CGameSocket::Create(const char* pszServerIP, int nServerPort)
{
	// 检查参数  
	if (pszServerIP == 0 || strlen(pszServerIP) > 15) {
		return false;
	}

	WSADATA wsaData;
	WORD version = MAKEWORD(2, 0);
	int ret = WSAStartup(version, &wsaData);//win sock start up  
	if (ret != 0) {
		return false;
	}

	// 创建主套接字  
	m_sockClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_sockClient == INVALID_SOCKET) {
		closeSocket();
		return false;
	}

	// 设置SOCKET为长链接 
	int optval = 1;
	if (setsockopt(m_sockClient, SOL_SOCKET, SO_KEEPALIVE, (char *)&optval, sizeof(optval)))
	{
		closeSocket();
		return false;
	}

	// 设置为非阻塞
	DWORD nMode = 1;
	int nRes = ioctlsocket(m_sockClient, FIONBIO, &nMode);
	if (nRes == SOCKET_ERROR) {
		closeSocket();
		return false;
	}


	unsigned long serveraddr = inet_addr(pszServerIP);
	if (serveraddr == INADDR_NONE)   // 检查IP地址格式错误  
	{
		closeSocket();
		return false;
	}

	sockaddr_in addr_in;
	memset((void *)&addr_in, 0, sizeof(addr_in));
	addr_in.sin_family = AF_INET;
	addr_in.sin_port = htons(nServerPort);
	addr_in.sin_addr.s_addr = serveraddr;

	if (connect(m_sockClient, (sockaddr *)&addr_in, sizeof(addr_in)) == SOCKET_ERROR) {
		if (hasError()) {
			closeSocket();
			return false;
		}
		else    // WSAWOLDBLOCK  
		{
			timeval timeout;
			timeout.tv_sec = 2;
			timeout.tv_usec = 0;
			fd_set writeset, exceptset;
			FD_ZERO(&writeset);
			FD_ZERO(&exceptset);
			FD_SET(m_sockClient, &writeset);
			FD_SET(m_sockClient, &exceptset);

			int ret = select(FD_SETSIZE, NULL, &writeset, &exceptset, &timeout);
			if (ret == 0 || ret < 0) {
				closeSocket();
				return false;
			}
			else  // ret > 0  
			{
				ret = FD_ISSET(m_sockClient, &exceptset);
				if (ret)     // or (!FD_ISSET(m_sockClient, &writeset)  
				{
					closeSocket();
					return false;
				}
			}
		}
	}

	m_nInbufLen = 0;
	m_nInbufStart = 0;
	m_nOutbufLen = 0;

	struct linger so_linger;
	so_linger.l_onoff = 1;
	so_linger.l_linger = 500;
	setsockopt(m_sockClient, SOL_SOCKET, SO_LINGER, (const char*)&so_linger, sizeof(so_linger));

	return true;
}

bool CGameSocket::SendMsg(const char* pBuf, int nSize)
{
	/*
	功能描述：向服务端发送数据（采用缓存，减少发送数据频率同时由于是采用非阻塞，需要缓存未发送完的内容）
	@param pBuf: 数据字符串（在本系统中约定为 json格式的数据，通过序列化后的数据）
	@param nSize: 数据长度
	@param bSendRightNow: 是否立即发送（默认为立即发送，如果本次传输的数据没有实时需要，可以将本字段设置flase，将会在下次发送是一起发送）
	*/

	// 由于数据是不定长的，所以需要在数据前加上表示数据长度的数据，暂定表示数据长度的数据长为LENGTHDATASIZE
	nSize = nSize + LENGTHDATASIZE;
	// 加上长度数据后发送数据
	char* tempBuf = new char[nSize + LENGTHDATASIZE];
	// 数据长度数据
	char digBuf[LENGTHDATASIZE] = { 0 };

	//进行合法性检测 
	if (pBuf == 0 || nSize <= 0) {
		return false;
	}

	if (m_sockClient == INVALID_SOCKET) {
		return false;
	}

	// 计算加上数据长度数据后总体发送数据长度
	for (int i = 0; i < LENGTHDATASIZE; i++) {
		digBuf[i] = nSize >> i * 8;
	}

	// 将数据长度数据添加到真实数据前端
	memcpy(tempBuf, digBuf, LENGTHDATASIZE);
	memcpy(tempBuf + LENGTHDATASIZE, pBuf, nSize);

	// 检查通讯消息包长度  
	int packsize = 0;
	packsize = nSize;

	// 如果发送出现溢出时，立即发送缓存中数据，来清空发送缓存数据
	if (m_nOutbufLen + nSize > OUTBUFSIZE) {
		// 立即发送OUTBUF中的数据，以清空OUTBUF。  
		Flush();
		// 发送数据后，还是溢出，说明Flush()出错了或者是发送缓存太小
		if (m_nOutbufLen + nSize > OUTBUFSIZE) {
			Destroy();
			return false;
		}
	}
	// 数据添加到BUF尾  
	memcpy(m_bufOutput + m_nOutbufLen, tempBuf, nSize);
	m_nOutbufLen += nSize;

	// 删除总体数据字符指针，避免内存泄露
	delete[] tempBuf;

	return true;
}

bool CGameSocket::ReceiveMsg(char* pBuf, int& nSize)
{
	int packsize = 0;
	//检查参数  
	if (pBuf == NULL || nSize <= 0) {
		return false;
	}
	//检查参数  
	if (m_sockClient == INVALID_SOCKET) {
		return false;
	}
	// 检查是否有一个消息(小于LENGTHDATASIZE则无法获取到消息长度)  
	if (m_nInbufLen < LENGTHDATASIZE) {
		//  如果没有请求成功  或者   如果没有数据则直接返回  
		if (!recvFromSock() || m_nInbufLen < LENGTHDATASIZE) {
			return false;
		}
	}

	// 计算些次包长度
	for (int i = 0; i < LENGTHDATASIZE; i++) {
		packsize += (unsigned char)m_bufInput[m_nInbufStart + i] * pow(256, i);
	}

	// 检测消息包尺寸错误 暂定最大16k （超过则认为错误，对缓存进行清空，重新开始）
	if (packsize <= 0 || packsize > _MAX_MSGSIZE) {
		m_nInbufLen = 0;
		m_nInbufStart = 0;
		return false;
	}

	// 检查消息是否完整(如果将要拷贝的消息大于此时缓冲区数据长度，需要再次请求接收剩余数据)  
	if (packsize > m_nInbufLen) {
		// 再次尝试获取一个完整包信息，如果失败或者依然不是整包大小，则返回，等下一帧再尝试获取 
		if (!recvFromSock() || packsize > m_nInbufLen) {
			return false;
		}
	}

	// 复制出一个消息  
	if (m_nInbufStart + packsize > INBUFSIZE) {
		// 如果一个消息有回环现象，即被拆成两份在环形缓冲区的头尾

		// 先拷贝环形缓冲区末尾的数据（去除消息包长度，只获取完整的数据包）
		int copylen = INBUFSIZE - m_nInbufStart;
		memcpy(pBuf, m_bufInput + m_nInbufStart + LENGTHDATASIZE, copylen - LENGTHDATASIZE);

		// 再拷贝环形缓冲区头部的剩余部分  
		memcpy((unsigned char *)pBuf + copylen, m_bufInput, packsize - copylen);
		nSize = packsize;
	}
	else {
		// 消息没有回环现象，则一次拷贝出去（去除消息包长度，只获取完整的数据包）
		memcpy(pBuf, m_bufInput + m_nInbufStart + LENGTHDATASIZE, packsize - LENGTHDATASIZE);
		nSize = packsize;
	}

	// 重新计算环形缓冲区头部位置  
	m_nInbufStart = (m_nInbufStart + packsize) % INBUFSIZE;
	m_nInbufLen -= packsize;
	return  true;
}

bool CGameSocket::hasError()
{
	int err = WSAGetLastError();
	if (err != WSAEWOULDBLOCK) {

		return true;
	}

	return false;
}

// 从网络中读取尽可能多的数据，实际取服务器发来的数据的地方  
bool CGameSocket::recvFromSock(void)
{
	if (m_nInbufLen >= INBUFSIZE || m_sockClient == INVALID_SOCKET) {
		return false;
	}

	// 接收第一段数据  
	int savelen, savepos;           // 数据要保存的长度和位置  
	if (m_nInbufStart + m_nInbufLen < INBUFSIZE) {
		savelen = INBUFSIZE - (m_nInbufStart + m_nInbufLen);
	}
	else {
		savelen = INBUFSIZE - m_nInbufLen;
	}

	// 缓冲区数据的末尾  
	savepos = (m_nInbufStart + m_nInbufLen) % INBUFSIZE;
	if (savepos + savelen > INBUFSIZE)
		return 0;

	int inlen = recv(m_sockClient, m_bufInput + savepos, savelen, 0);
	if (inlen > 0) {
		// 有接收到数据  
		m_nInbufLen += inlen;

		if (m_nInbufLen > INBUFSIZE) {
			return false;
		}

		// 接收第二段数据(一次接收没有完成，接收第二段数据)  
		if (inlen == savelen && m_nInbufLen < INBUFSIZE) {
			int savelen = INBUFSIZE - m_nInbufLen;
			int savepos = (m_nInbufStart + m_nInbufLen) % INBUFSIZE;
			if (savepos + savelen > INBUFSIZE)
				return 0;

			inlen = recv(m_sockClient, m_bufInput + savepos, savelen, 0);
			if (inlen > 0) {
				m_nInbufLen += inlen;
				if (m_nInbufLen > INBUFSIZE) {
					return false;
				}
			}
			else if (inlen == 0) {
				Destroy();
				return false;
			}
			else {
				// 连接已断开或者错误（包括阻塞）  
				if (hasError()) {
					Destroy();
					return false;
				}
			}
		}
	}
	else if (inlen == 0) {
		Destroy();
		return false;
	}
	else {
		// 连接已断开或者错误（包括阻塞）  
		if (hasError()) {
			Destroy();
			return false;
		}
	}
	return true;
}

bool CGameSocket::Flush(void)
{
	/*
	功能描述：
	*/
	if (m_sockClient == INVALID_SOCKET) {
		return false;
	}

	if (m_nOutbufLen <= 0) {
		return true;
	}

	// 发送一段数据  
	int outsize;
	outsize = send(m_sockClient, m_bufOutput, m_nOutbufLen, 0);
	if (outsize > 0) {
		// 删除已发送的部分  
		if (m_nOutbufLen - outsize > 0) {
			memcpy(m_bufOutput, m_bufOutput + outsize, m_nOutbufLen - outsize);
		}

		m_nOutbufLen -= outsize;

		if (m_nOutbufLen < 0) {
			return false;
		}
	}
	else {
		if (hasError()) {
			Destroy();
			return false;
		}
	}

	return true;
}

bool CGameSocket::Check(void)
{
	// 检查状态  
	if (m_sockClient == INVALID_SOCKET) {
		return false;
	}

	char buf[1];

	// 查看接收缓存区数据量
	int ret = recv(m_sockClient, buf, 1, MSG_PEEK);
	if (ret == 0) {
		Destroy();
		return false;
	}
	else if (ret < 0) {
		if (hasError()) {
			Destroy();
			return false;
		}
		else {    // 阻塞  
			return true;
		}
	}
	else {    // 有数据  
		return true;
	}

	return true;
}

void CGameSocket::Destroy(void)
{
	// 关闭  
	struct linger so_linger;
	so_linger.l_onoff = 1;
	so_linger.l_linger = 500;
	int ret = setsockopt(m_sockClient, SOL_SOCKET, SO_LINGER, (const char*)&so_linger, sizeof(so_linger));

	closeSocket();

	m_sockClient = INVALID_SOCKET;
	m_nInbufLen = 0;
	m_nInbufStart = 0;
	m_nOutbufLen = 0;

	memset(m_bufOutput, 0, sizeof(m_bufOutput));
	memset(m_bufInput, 0, sizeof(m_bufInput));
}







