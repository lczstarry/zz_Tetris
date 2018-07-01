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

#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__


#include "cocos2d.h"
#include "GameBackground.h"
#include "GameNet.h"



class HelloWorld : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();
	bool m_begin;
	virtual bool init();
	int pscore;

	string Countnum;
	bool timeout;
	bool restart;
	bool isrestart;
	bool isgameover;
	int score1;
	int score2;
	int score3;
	
	void menuCloseCallback(cocos2d::Ref* pSender);
	void gameStart(Ref * pSender);
	void reStart();
	void initAll();
	void packInfo(bool begin,bool restart,bool isgameover,int pscore);
	CREATE_FUNC(HelloWorld);
	// Ã¿Ö¡µ÷ÓÃ
	void update(float delta);
	GameBackground * m_pBackground;

protected:
	cocos2d::CCMenu* m_pStartMenu;
	cocos2d::CCMenu* m_pControlMenu;
	
	//cocos2d::CCSprite* m_pStartSprite;
	cocos2d::CCMenu* m_pPauseMenu;

private:
	
	CGameNet * pGameNet = NULL;
};

#endif // __HELLOWORLD_SCENE_H__
