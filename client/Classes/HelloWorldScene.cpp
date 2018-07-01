/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"


#include <iostream>
#include "BackgroundNet.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "json/stringbuffer.h"
#include "json/writer.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}
void HelloWorld::update(float delta)
{
	/*
	功能描述: 通过每帧更新，驱动网络协议管理对象流程处理（详情可看MainLoop函数内容）
	*/
	pGameNet->MainLoop();
	
	if (isrestart)
	{
		restart = false;
		isrestart = false;
	}

	packInfo(m_begin,restart,isgameover,pscore);
	
	if (timeout)
	{
		m_pBackground->gameOver();
	}

	

}
// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{


	// 初始化网络管理模块并连接服务器
	pGameNet = CGameNet::GetInstance();
	pGameNet->init("127.0.0.1", 12080);

    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();


	initAll();
	CCMenuItemFont* startItem = CCMenuItemFont::create("start", this, menu_selector(HelloWorld::gameStart));
	CCMenuItemFont* exitItem = CCMenuItemFont::create("exit", this, menu_selector(HelloWorld::menuCloseCallback));
	//m_pStartSprite = cocos2d::CCSprite::create("cat.png");
	//m_pStartSprite->setPosition(cocos2d::CCPoint(-100, 520));
	//addChild(m_pStartSprite);
	CCActionInterval * actionTo = CCMoveTo::create(1, cocos2d::CCPoint(160, 520));
	//m_pStartSprite->runAction(actionTo);
	m_pStartMenu = CCMenu::create(startItem, exitItem, NULL);
	exitItem->setPosition(0, -70);
	m_pStartMenu->setPositionX(160);
	m_pStartMenu->setPosition(cocos2d::CCPoint(-100, 400));
	this->addChild(m_pStartMenu);
	CCActionInterval * actionTo2 = CCMoveTo::create(1, cocos2d::CCPoint(160, 400));
	m_pStartMenu->runAction(actionTo2);

	Countnum = "0";
	timeout = false;
	restart = false;
	isrestart = false;
	isgameover = false;
	pscore = -1;
	score1 = 0;
	score2 = 0;
	score3 = 0;

	//pStartItem -> setScale(0.75);
	m_begin = false;
	this->scheduleUpdate();
    return true;
}


void HelloWorld::packInfo(bool begin,bool restart,bool isgameover,int pscore)
{
	/*
	功能描述： 将需要向服务端传输的信息进行整合，这里整合成了json文件，整合完后
	*/

	//生成Json串
	rapidjson::Document jsonDoc;
	rapidjson::Document::AllocatorType &allocator = jsonDoc.GetAllocator();
	jsonDoc.SetObject();

	//添加属性
	jsonDoc.AddMember("action", "updatepos", allocator);
	jsonDoc.AddMember("BeginCountDown", begin, allocator);
	jsonDoc.AddMember("Restart", restart, allocator);
	jsonDoc.AddMember("isgameover", isgameover, allocator);
	jsonDoc.AddMember("pscore",pscore,allocator);

	//生成array
	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	jsonDoc.Accept(writer);

	std::string strJson = buffer.GetString();
	C2GSUpdatePos(strJson.c_str());

	pscore = -1;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}
void HelloWorld::reStart()
{
	if (m_pBackground)
	{
		m_pBackground->removeFromParent();
		m_pBackground = NULL;
	}
	
	if (m_pPauseMenu)
	{
		m_pPauseMenu->removeFromParent();
		m_pPauseMenu = NULL;
		
	}
	if (m_pControlMenu)
	{
		m_pControlMenu->removeFromParent();
		m_pControlMenu = NULL;
	}
	initAll();
	//m_begin = true;
	restart = true;
	m_pBackground->initCurtains();

	m_pBackground->gameStart();
}
void HelloWorld::gameStart(Ref * pSender)
{
	//m_pStartMenu -> removeFromParent();

	cocos2d::CCActionInterval *moveAway = cocos2d::CCMoveTo::create(0.3, cocos2d::CCPoint(-100, 520));
	//m_pStartSprite->runAction(moveAway);

	cocos2d::CCActionInterval *moveAway2 = cocos2d::CCMoveTo::create(0.3, cocos2d::CCPoint(-100, 400));
	m_pStartMenu->runAction(moveAway2);
	m_pBackground->initCurtains();

	m_pBackground->gameStart();

	m_begin = true;
	packInfo(m_begin,restart,isgameover,pscore);
}
void HelloWorld::initAll()
{

	m_pBackground = GameBackground::create();
	m_pBackground->setScaleY(0.8);
	addChild(m_pBackground);

	CCMenuItemImage* pLeftItem = CCMenuItemImage::create("zuo.png", "zuo.png", m_pBackground, menu_selector(GameBackground::moveLeft));
	pLeftItem->setScale(0.6);
	CCMenuItemImage* pRightItem = CCMenuItemImage::create("you.png", "you.png", m_pBackground, menu_selector(GameBackground::moveRight));
	pRightItem->setScale(0.6);
	CCMenuItemImage* pTransItem = CCMenuItemImage::create("bian.png", "bian.png", m_pBackground, menu_selector(GameBackground::rotation));
	pTransItem->setScale(0.6);
	CCMenuItemImage* pAcdownItem = CCMenuItemImage::create("xia.png", "xia.png", m_pBackground, menu_selector(GameBackground::accelerateDown));
	pAcdownItem->setScale(0.6);
	m_pControlMenu = CCMenu::create(pLeftItem, pRightItem, pTransItem, pAcdownItem, NULL);
	this->addChild(m_pControlMenu);
	m_pControlMenu->setPosition(130, 20);
	pLeftItem->setPosition(ccp(-60, 25));
	pLeftItem->setScale(0.2);
	pAcdownItem->setPosition(ccp(270, 25));
	pAcdownItem->setScale(0.2);
	//pStartItem->setPosition(ccp(-30,0));
	pRightItem->setPosition(ccp(160, 25));
	pRightItem->setScale(0.2);
	pTransItem->setPosition(ccp(50, 25));
	pTransItem->setScale(0.25);
	CCMenuItemImage* pauseItem = CCMenuItemImage::create("pause.png", "pause.png", m_pBackground, menu_selector(GameBackground::gamePause));
	m_pPauseMenu = cocos2d::CCMenu::create(pauseItem, NULL);
	addChild(m_pPauseMenu);
	m_pPauseMenu->setPosition(cocos2d::CCPoint(380, 200));
}





