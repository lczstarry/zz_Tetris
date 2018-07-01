#include "GameNet.h"

/*
�๦�������������շ��������Э�����ݷַ�
*/
CGameNet * CGameNet::pGameNet = NULL;

void CGameNet::init(const char * pszServerIP, int nServerPort)
{
	/*
	��������: �������������ӵ�����˳�ʼ��
	*/
	pGameSocket = new CGameSocket();
	if (!pGameSocket->Create(pszServerIP, nServerPort)) {
		cout << "init socket obj error" << endl;
	}
}

// �����жϻص�
void CGameNet::onConnectFail()
{
	cout << "net connect fail" << endl;
}

void CGameNet::MainLoop()
{
	/*
	���������� ����Э������ѭ����һ��Ϊÿ֡���ã���������socket��������շ�������������Э�鶯������ַ�
	*/
	if (!pGameSocket) {
		return;
	}
	if (!pGameSocket->Check()) {
		pGameSocket = NULL;
		// ������
		onConnectFail();
		return;
	}

	// �������ݣ��������������Ϣ��
	pGameSocket->Flush();

	while (true) {
		char tempBuffer[16 * 1024] = { 0 };
		int nSize = sizeof(tempBuffer);
		// ѭ��������Ϣ��ֱ�����յ�ǰ�ɽ��յ�������Ϣ
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
	����������Э�����ݷַ������յ�����˴�����json���ݣ�sJson��ʱ��ȡ�����е�action�ֶΣ�ͨ�����ֶηַ�����ͬ��������
	*/
	// ��ȡjson�е�action�ֶ�
	string sAction = GetAction(sJson);
	if (sAction == "") {
		cout << "invalid net pack cmd " << endl;
		return;
	}

	// ��Э�麯����ϵ�ֵ��в���
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
	����������ͨ���õ�json�ļ���ȡ����Э���action
	@param sJson�����η���˷�������json����char�ַ�������
	*/

	// �����л�josn
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
	������������ȡ����Э�������
	*/
	if (pGameNet == NULL) {
		pGameNet = new CGameNet();
	}
	return pGameNet;
}

CGameSocket * CGameNet::GetSocket()
{
	/*
	������������ȡsocket����
	*/
	return pGameSocket;
}




