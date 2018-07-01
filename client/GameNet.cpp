#include "GameNet.h"

/*
类功能描述：网络收发包及相关协议数据分发
*/
CGameNet * CGameNet::pGameNet = NULL;

void CGameNet::init(const char * pszServerIP, int nServerPort)
{
	/*
	功能描述: 网络管理对象连接到服务端初始化
	*/
	pGameSocket = new CGameSocket();
	if (!pGameSocket->Create(pszServerIP, nServerPort)) {
		cout << "init socket obj error" << endl;
	}
}

// 网络中断回调
void CGameNet::onConnectFail()
{
	cout << "net connect fail" << endl;
}

void CGameNet::MainLoop()
{
	/*
	功能描述： 网络协议驱动循环，一般为每帧调用，用来驱动socket对象进行收发包处理与网络协议动作命令分发
	*/
	if (!pGameSocket) {
		return;
	}
	if (!pGameSocket->Check()) {
		pGameSocket = NULL;
		// 掉线了
		onConnectFail();
		return;
	}

	// 发送数据（向服务器发送消息）
	pGameSocket->Flush();

	while (true) {
		char tempBuffer[16 * 1024] = { 0 };
		int nSize = sizeof(tempBuffer);
		// 循环接收消息，直到接收当前可接收的所有消息
		if (!pGameSocket->ReceiveMsg(tempBuffer, nSize))
			break;

		if (strlen(tempBuffer) > 0) {
			onServerCmd(tempBuffer);
		}
	}
}

void CGameNet::onServerCmd(const char * sJson)
{
	/*
	功能描述：协议数据分发（接收到服务端传来的json数据（sJson）时，取出其中的action字段，通过此字段分发给不同函数处理）
	*/
	// 获取json中的action字段
	string sAction = GetAction(sJson);
	if (sAction == "") {
		cout << "invalid net pack cmd " << endl;
		return;
	}

	// 在协议函数关系字典中查找
	funcIter = funcMap.find(sAction);
	if (funcIter != funcMap.end()) {
		funcIter->second(sJson);
	}
	else {
		cout << "not find target func " << sAction << endl;
	}
}

string CGameNet::GetAction(const char* sJson)
{
	/*
	功能描述：通过得的json文件获取本次协议的action
	@param sJson：本次服务端发送来的json内容char字符串数组
	*/

	// 反序列化josn
	rapidjson::Document pJsonDoc;
	pJsonDoc.Parse<0>(sJson);

	if (pJsonDoc.HasParseError()) {
		cout << "GetAction error, when parse json data";
		return "";
	}

	if (!pJsonDoc.IsObject())
		return "";

	if (!pJsonDoc.HasMember("action")) {
		cout << "GetAction error, not find key word action in json data";
		return "";
	}

	const rapidjson::Value &action = pJsonDoc["action"];

	return action.GetString();
}

bool CGameNet::SendMsg(const char * pBuf)
{
	if (pGameSocket)
		return pGameSocket->SendMsg(pBuf, strlen(pBuf));
	else {
		cout << "net error, plesse check !!!" << endl;
		return false;
	}

}

CGameNet * CGameNet::GetInstance()
{
	/*
	功能描述：获取网络协议管理单例
	*/
	if (pGameNet == NULL) {
		pGameNet = new CGameNet();
	}
	return pGameNet;
}

CGameSocket * CGameNet::GetSocket()
{
	/*
	功能描述：获取socket对象
	*/
	return pGameSocket;
}




