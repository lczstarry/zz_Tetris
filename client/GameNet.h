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
	// socket�������
	CGameSocket * pGameSocket = NULL;
	// ����Э��������
	static CGameNet* pGameNet;
	// ˽�л���������������

	// ����δʵ�ֿ����븳ֵ���캯������������С����Լ������������ԣ�

public:

	// �����жϵ��� 
	void onConnectFail();
	// �����շ���ѭ��
	void MainLoop();
	// Э�����ݷַ������յ�����˴�����json����ʱ��ȡ�����е�action�ֶΣ�ͨ�����ֶηַ�����ͬ��������
	void onServerCmd(const char* sJson);
	// ��ʼ����������
	void init(const char* pszServerIP, int nServerPort);
	// ��ȡ����Э�����ַ����� 
	static CGameNet *GetInstance();
	// ��ȡsocket����
	CGameSocket * GetSocket();
	// �ӷ���˷��ص�json���ݰ��л�ȡЭ�鶯��
	string GetAction(const char* sJson);
	// ����json���ݵ������
	bool SendMsg(const char* pBuf);
};


