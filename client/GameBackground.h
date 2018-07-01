#pragma once

#include "cocos2d.h"
#include "EditBoxManager.h"

//#include <iostream>
//#include "BackgroundNet.h"
//#include "json/rapidjson.h"
//#include "json/document.h"
//#include "json/stringbuffer.h"
//#include "json/writer.h"
#include<string>
using namespace std;
using namespace cocos2d;
USING_NS_CC;


#define WIDTH 10
#define HEIGHT 26
typedef struct
{
	int x;
	int y;
}NodePos;
class GameBackground : 
	public cocos2d::CCLayer
{
public:
	GameBackground(void);
	~GameBackground(void);
	virtual bool init();
	void moveLeft(Ref* pSender);
	void moveRight(Ref* pSender);
	void rotation(Ref* pSender);
	void gameStart();
	void gamePause(Ref * pSender);
	void moveDown(float);
	void accelerateDown(Ref* pSender);
	void initCurtains();
	bool createBoxOne();
	bool createBoxTwo();
	bool createBoxThree();
	bool createBoxFour();
	bool createBoxFive();
	bool createBoxSix();
	bool createBoxSeven();
	void createNextBox(int n);
	bool isCurtainEnabled(NodePos);
	void finishdown();
	void upDateCurtains();
	void sortByLeft();
	void sortByRight();
	void sortByDown();
	void calculate();
	void gameOver();
	void exitGame(Ref* pSender);
	void reStart(Ref* pSender);
	void hide(Node*);
	CREATE_FUNC(GameBackground);

	void CountDown(string Count);
	
	int score;
	//EditBox * mEditBox;
	//CCTextFieldTTF *text;
	//EditBoxManager* editBoxLayer;

	void getBoxText(Ref* pSender);


private:
	cocos2d::CCSprite * m_pSprites[HEIGHT][WIDTH];
	bool m_bArray[HEIGHT][WIDTH];
	cocos2d::CCLayer * m_pScoreBackGround;
	cocos2d::CCLabelTTF* m_pscoreLabel;

	cocos2d::CCLabelTTF* m_score1Label;
	cocos2d::CCLabelTTF* m_score2Label;
	cocos2d::CCLabelTTF* m_score3Label;

	cocos2d::CCLabelTTF* m_ptimeLabel;

	cocos2d::CCSprite * m_pSpritesNext[4][4];
	bool m_bSuccess;
	NodePos m_NodesArray[4];
	NodePos m_NodeRotate;
	int m_nCurrentType;
	int m_nCount;
	bool isMovedArray[4];
	bool m_isPaused;
	int m_nScoreNum;
	cocos2d::CCMenu *m_pGameOverMenu;
	cocos2d::CCMenu *m_pGamePauseMenu;
	cocos2d::CCLayer *m_pPauseLayer;
	bool m_bPauseCtrl;
	bool m_bControl;
	cocos2d::CCSprite * m_boom;
	cocos2d::CCSprite * m_boom2;

	int mscore1;
	int mscore2;
	int mscore3;

	//EditBoxManager* editBox;
};




