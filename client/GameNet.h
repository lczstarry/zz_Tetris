#pragma once
#include "GameCustomSocket.h"
#include <map>
#include <functional>
#include <string>
#include "json/rapidjson.h"
#include "json/document.h"
#include "json/stringbuffer.h"
#include "json/writer.h"

using namespace std;
extern map<string, function<void(const char* sJson)>> funcMap;
extern map<string, function<void(const char* sJson)>>::iterator funcIter;


class CGameNet
{
private:
	// socket处理对象
	CGameSocket * pGameSocket = NULL;
	// 网络协议管理对象
	static CGameNet* pGameNet;
	// 私有化构造下析构函数

	// 这里未实现拷贝与赋值构造函数会有隐患（小伙伴自己完善这里试试）

public:

	// 网络中断调用 
	void onConnectFail();
	// 网络收发主循环
	void MainLoop();
	// 协议数据分发（接收到服务端传来的json数据时，取出其中的action字段，通过此字段分发给不同函数处理）
	void onServerCmd(const char* sJson);
	// 初始化网络连接
	void init(const char* pszServerIP, int nServerPort);
	// 获取网络协议管理分发对象 
	static CGameNet *GetInstance();
	// 获取socket对象
	CGameSocket * GetSocket();
	// 从服务端发回的json数据包中获取协议动作
	string GetAction(const char* sJson);
	// 发送json数据到服务端
	bool SendMsg(const char* pBuf);
};


