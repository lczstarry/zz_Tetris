#pragma once
#include <string>
#include "cocos2d.h"
#include "GameNet.h"
#include "HelloWorldScene.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "json/stringbuffer.h"
#include "json/writer.h"


using namespace std;
USING_NS_CC;

 //static string Countnum = "10" ;
// �ͻ��˵���������ݷ��ͺ��� 
void C2GSUpdatePos(const char * msg);

// ����˵��ͻ������ݴ�����
void GS2CShowServerInfo(const char *msg);

