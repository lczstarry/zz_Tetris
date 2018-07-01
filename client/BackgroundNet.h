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
// 客户端到服务端数据发送函数 
void C2GSUpdatePos(const char * msg);

// 服务端到客户端数据处理函数
void GS2CShowServerInfo(const char *msg);

