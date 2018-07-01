#include "BackgroundNet.h"
#include "iostream"
void C2GSUpdatePos(const char * msg)
{
	// ���start��ť  ��ʼ����ʱ������  �������Ϣ���ݵ������
	CGameNet::GetInstance()->SendMsg(msg);
}

void GS2CShowServerInfo(const char * msg)
{
	/*
	�������������շ���˷��صĵ���ʱ����
	*/
	rapidjson::Document pJsonDoc;
	pJsonDoc.Parse<0>(msg);

	if (pJsonDoc.HasParseError()) {
		cout << "error, when parse json data in GS2CShowServerInfo";
		return;
	}

	if (!pJsonDoc.IsObject())
		return;

	if (!pJsonDoc.HasMember("Count")) {
		cout << "error, not find key word action in json data in GS2CShowServerInfo";
		return;
	}

	if (!pJsonDoc.HasMember("timeout")) {
		cout << "error, not find key word action in json data in GS2CShowServerInfo";
		return;
	}

	if (!pJsonDoc.HasMember("isrestart")) {
		cout << "error, not find key word action in json data in GS2CShowServerInfo";
		return;
	}
	if (!pJsonDoc.HasMember("score1")) {
		cout << "error, not find key word action in json data in GS2CShowServerInfo";
		return;
	}
	if (!pJsonDoc.HasMember("score2")) {
		cout << "error, not find key word action in json data in GS2CShowServerInfo";
		return;
	}
	if (!pJsonDoc.HasMember("score3")) {
		cout << "error, not find key word action in json data in GS2CShowServerInfo";
		return;
	}


	// ��ȡ����˷��ص�json�ļ��е��ֶΣ�Count��int �ͣ�timeout �� bool ��
	const rapidjson::Value &CountD = pJsonDoc["Count"];
	const rapidjson::Value &timeoutD = pJsonDoc["timeout"];
	const rapidjson::Value &isrestartD = pJsonDoc["isrestart"];
	const rapidjson::Value &score1D = pJsonDoc["score1"];
	const rapidjson::Value &score2D = pJsonDoc["score2"];
	const rapidjson::Value &score3D = pJsonDoc["score3"];
	// ��ȡ��ǰ��ʾ��scene(Ҳ���� HelloWorldScene�е�HelloWorld) 
	HelloWorld* pCurrentScene = (HelloWorld*)CCDirector::getInstance()->getRunningScene();

	//if (flag.GetBool())
	//	pCurrentScene->ShowServerInfo("server info: has detected collision");
	//else
	//	pCurrentScene->ShowServerInfo("server info: not detected collision");
	
	
	//pCurrentScene->Countnum = to_string(CountD.GetInt());
	
	pCurrentScene->m_pBackground->CountDown(to_string(CountD.GetInt()));
	pCurrentScene->timeout=timeoutD.GetBool();
	pCurrentScene->isrestart = isrestartD.GetBool();
	pCurrentScene->score1 = score1D.GetInt();
	pCurrentScene->score2 = score2D.GetInt();
	pCurrentScene->score3 = score3D.GetInt();

}

