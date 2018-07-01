#pragma once  

#include <windows.h>  
#include <WinSock.h>  
#include <iostream>
#include <cmath>
using namespace std;

#define _MAX_MSGSIZE 16 * 1024      // �����Ϣ��С  
#define INBUFSIZE   (64*1024)       //�������ݵĻ����С
#define OUTBUFSIZE  (8*1024)        //�������ݵĻ����С
#define LENGTHDATASIZE 4            //�������ݳ��ȱ�ʶ���ݵĳ���

class CGameSocket {
private:
	// �������ж�ȡ�����ܶ������ 
	bool recvFromSock(void);
	// �Ƿ�������ע�⣬�첽ģʽδ��ɷǴ���  
	bool hasError();
	void closeSocket();

	// socket����
	SOCKET m_sockClient;

	// �������ݻ���  
	char m_bufOutput[OUTBUFSIZE];
	// �������ݻ��嵱ǰ���ݳ���
	int m_nOutbufLen;

	// �������ݻ��� 
	char m_bufInput[INBUFSIZE];
	// ���ջ��浱ǰ���ݳ���
	int m_nInbufLen;
	// �������
	int m_nInbufStart;

public:
	CGameSocket(void);
	bool Create(const char* pszServerIP, int nServerPort);
	// ���Է������ݵ�����ˣ�һ����ȼ��뵽�����У�
	bool SendMsg(const char* pBuf, int nSize);
	// ���շ���˷���������
	bool ReceiveMsg(char* pBuf, int& nSize);
	// �������ͻ���������
	bool Flush(void);
	// ��⵱ǰ����״̬
	bool Check(void);
	// ����socket
	void Destroy(void);
	// ��ȡsocket����ӿ�
	SOCKET GetSocket(void) const { return m_sockClient; }


};
