#pragma once  

#include <windows.h>  
#include <WinSock.h>  
#include <iostream>
#include <cmath>
using namespace std;

#define _MAX_MSGSIZE 16 * 1024      // 最大消息大小  
#define INBUFSIZE   (64*1024)       //接收数据的缓存大小
#define OUTBUFSIZE  (8*1024)        //发送数据的缓存大小
#define LENGTHDATASIZE 4            //发送数据长度标识数据的长度

class CGameSocket {
private:
	// 从网络中读取尽可能多的数据 
	bool recvFromSock(void);
	// 是否发生错误，注意，异步模式未完成非错误  
	bool hasError();
	void closeSocket();

	// socket对象
	SOCKET m_sockClient;

	// 发送数据缓冲  
	char m_bufOutput[OUTBUFSIZE];
	// 发送数据缓冲当前数据长度
	int m_nOutbufLen;

	// 接收数据缓存 
	char m_bufInput[INBUFSIZE];
	// 接收缓存当前数据长度
	int m_nInbufLen;
	// 数据起点
	int m_nInbufStart;

public:
	CGameSocket(void);
	bool Create(const char* pszServerIP, int nServerPort);
	// 尝试发送数据到服务端（一般会先加入到缓存中）
	bool SendMsg(const char* pBuf, int nSize);
	// 接收服务端发来的数据
	bool ReceiveMsg(char* pBuf, int& nSize);
	// 立即发送缓存中数据
	bool Flush(void);
	// 检测当前网络状态
	bool Check(void);
	// 销毁socket
	void Destroy(void);
	// 获取socket对象接口
	SOCKET GetSocket(void) const { return m_sockClient; }


};
