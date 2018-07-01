#include "GameBackground.h"
using namespace std;
#include "HelloWorldScene.h"

USING_NS_CC;

#define FONT_NAME "fonts/Marker Felt.ttf"  
#define FONT_SIZE 26  
GameBackground::GameBackground(void)
{
}


GameBackground::~GameBackground(void)
{
}

void GameBackground::gamePause(Ref * pSender)
{
	if (!m_bPauseCtrl)
		return;
	cocos2d::CCDirector *director = cocos2d::CCDirector::sharedDirector();
	if (!m_isPaused)
	{

		

		director->pause();
		m_isPaused = true;
		//	
		m_pPauseLayer = cocos2d::CCLayer::create();
		cocos2d::CCMenuItemImage * continueItem = cocos2d::CCMenuItemImage::create("continueA.png", "continueB.png", this, menu_selector(GameBackground::gamePause));
		cocos2d::CCMenuItemImage * exitGame = cocos2d::CCMenuItemImage::create("gameover.png", "gameover.png", this, menu_selector(GameBackground::exitGame));
		cocos2d::CCMenuItemImage * newGame = cocos2d::CCMenuItemImage::create("newgameA.png", "newgameB.png", this, menu_selector(GameBackground::reStart));
		m_pGamePauseMenu = cocos2d::CCMenu::create(continueItem, newGame, exitGame, NULL);
		//m_pGamePauseMenu -> setAnchorPoint(cocos2d::CCPoint(0.5,0.5));
		continueItem->setPositionY(20);
		continueItem->setPositionX(-20);
		exitGame->setScaleX(0.5);
		exitGame->setPositionY(-180);
		exitGame->setPositionX(-20);
		newGame->setPositionX(-20);
		newGame->setPositionY(-80);
		m_pPauseLayer->addChild(m_pGamePauseMenu);
		//m_pGamePauseMenu->setPosition(cocos2d::CCPoint(-50,-100));
		addChild(m_pPauseLayer);
		m_pPauseLayer->setPosition(cocos2d::CCPoint(0, 150));
		m_bControl = false;
	}
	else
	{
		


		cocos2d::CCActionInterval * scaleBy = CCScaleBy::create(0.5, 0.1);
		//cocos2d::CCActionInterval * scaleByBack = scaleBy->reverse();
		cocos2d::CCActionInstant * removSelf = CCRemoveSelf::create();
		m_pPauseLayer->runAction(cocos2d::CCSequence::create(scaleBy, removSelf, NULL));

		//m_pGamePauseMenu -> removeFromParent();
		director->resume();
		m_isPaused = false;
		m_bControl = true;
	}
}

void GameBackground::initCurtains()
{
	float distanceX = 0;
	float distanceY = 0;
	for (int i = 0; i < HEIGHT; i++)
	{

		for (int j = 0; j < WIDTH; j++)
		{
			/***********************/
			//m_pSprites[i][j] = cocos2d::CCSprite::create("node.png",cocos2d::CCRect(0,0,30,30));
			m_pSprites[i][j] = cocos2d::CCSprite::create("node.png");
			m_pSprites[i][j]->setScale(1.5);
			m_pSprites[i][j]->setAnchorPoint(cocos2d::CCPoint(0, 0));
			m_pSprites[i][j]->setPosition(cocos2d::CCPoint(distanceX, distanceY));
			m_pSprites[i][j]->setVisible(false);
			m_bArray[i][j] = true;
			addChild(m_pSprites[i][j]);
			distanceX += 30;
		}
		distanceX = 0;
		distanceY += 30;
	}
	m_bControl = true;
	m_bPauseCtrl = true;
}
bool GameBackground::init()
{
	if (!CCLayer::init())
		return false;
	cocos2d::CCSprite * backgroundSprites = cocos2d::CCSprite::create("bg.png");
	cocos2d::ccColor4B  color = { 255,255,255,23 };
	cocos2d::CCLayerColor *backcolor = cocos2d::CCLayerColor::create(color);
	backgroundSprites->setPosition(cocos2d::CCPoint(160, 480));
	//backgroundSprites->setScale(2);

	score = 0;
	mscore1 = 0;
	mscore2 = 0;
	mscore3 = 0;


	addChild(backgroundSprites);
	addChild(backcolor);
	cocos2d::ccColor4B color2 = { 25,25,255,23 };
	m_pScoreBackGround = cocos2d::CCLayerColor::create(color2);
	m_pScoreBackGround->setPositionX(30 * WIDTH);
	addChild(m_pScoreBackGround);
	cocos2d::CCLabelTTF* nextLabel = cocos2d::CCLabelTTF::create();
	nextLabel->setString("Next :");
	nextLabel->setFontSize(40);
	m_pScoreBackGround->addChild(nextLabel);
	nextLabel->setPosition(cocos2d::CCPoint(60, 700));
	float posX = 40;
	//float posY = 240;
	float posY = 500;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			m_pSpritesNext[i][j] = cocos2d::CCSprite::create("node.png");
			m_pSpritesNext[i][j]->setScale(1.5);
			m_pSpritesNext[i][j]->setPosition(cocos2d::CCPoint(posX, posY));
			m_pScoreBackGround->addChild(m_pSpritesNext[i][j]);
			posX += 30;
		}
		posX = 40;
		posY += 30;
	}
	//
	cocos2d::CCLabelTTF* scoreTip = cocos2d::CCLabelTTF::create();
	scoreTip->setString("Score :");
	scoreTip->setFontSize(40);
	m_pScoreBackGround->addChild(scoreTip);
	scoreTip->setPosition(cocos2d::CCPoint(70, 110));
	//
	m_pscoreLabel = cocos2d::CCLabelTTF::create();
	m_pscoreLabel->setString("0");
	m_pscoreLabel->setFontSize(40);
	m_pScoreBackGround->addChild(m_pscoreLabel);
	m_pscoreLabel->setPosition(cocos2d::CCPoint(40, 20));


	//auto s = Size(250, 50);  //设置编辑框大小  
	//						 //Scale9Sprite类似android上的9图工具,可对图片进行拉伸而不失真  
	//auto m9pic = Scale9Sprite::create("bg.png");
	//mEditBox = EditBox::create(s, m9pic);

	//
	//
	cocos2d::CCLabelTTF* timeTip = cocos2d::CCLabelTTF::create();
	timeTip->setString("CountDown:");
	timeTip->setFontSize(40);
	m_pScoreBackGround->addChild(timeTip);
	timeTip->setPosition(cocos2d::CCPoint(70, 300));
	//
	m_ptimeLabel = cocos2d::CCLabelTTF::create();
	m_ptimeLabel->setString("0");
	m_ptimeLabel->setFontSize(40);
	m_pScoreBackGround->addChild(m_ptimeLabel);
	m_ptimeLabel->setPosition(cocos2d::CCPoint(40, 250));
	//
	//






	m_boom = cocos2d::CCSprite::create("boom1.png");
	m_boom->setVisible(false);
	m_pScoreBackGround->addChild(m_boom);
	m_boom->setPosition(cocos2d::CCPoint(70, 240));
	//
	m_boom2 = cocos2d::CCSprite::create("boom1.png");
	m_boom2->setVisible(false);
	m_pScoreBackGround->addChild(m_boom2);
	m_boom2->setPosition(cocos2d::CCPoint(90, 100));
	for (int i = 0; i < 4; i++)
	{
		m_NodesArray[i].x = 0;
		m_NodesArray[i].y = 0;

	}
	for (int i = 0; i < 4; i++)
	{
		isMovedArray[i] = false;
	}
	m_nCurrentType = 0;
	m_nCount = 0;
	m_isPaused = false;
	m_nScoreNum = 0;
	m_bSuccess = true;
	m_bControl = false;
	m_bPauseCtrl = false;
	return true;
}





void GameBackground::createNextBox(int n)
{
	//创建棍子
	if (n == 0)
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
			{
				if (j != 1)
					m_pSpritesNext[i][j]->setVisible(false);
				else
					m_pSpritesNext[i][j]->setVisible(true);
			}
	else
		//创建s
		if (n == 1)
		{
			for (int i = 0; i < 4; i++)
				for (int j = 0; j < 4; j++)
				{
					if (j == 0 && i == 1 || i == 1 && j == 1 || j == 1 && i == 2 || j == 2 && i == 2)
						m_pSpritesNext[i][j]->setVisible(true);
					else
						m_pSpritesNext[i][j]->setVisible(false);
				}
		}
		else//创建2
			if (n == 2)
			{
				for (int i = 0; i < 4; i++)
					for (int j = 0; j < 4; j++)
					{
						if (j == 0 && i == 2 || j == 1 && i == 2 || j == 1 && i == 1 || j == 2 && i == 1)
						{
							m_pSpritesNext[i][j]->setVisible(true);
						}
						else
							m_pSpritesNext[i][j]->setVisible(false);
					}
			}
			else//创建L
				if (n == 3)
				{
					for (int i = 0; i < 4; i++)
						for (int j = 0; j < 4; j++)
						{
							if (j == 0 && i == 2 || j == 0 && i == 1 || j == 1 && i == 1 || j == 2 && i == 1)
							{
								m_pSpritesNext[i][j]->setVisible(true);
							}
							else
							{
								m_pSpritesNext[i][j]->setVisible(false);
							}
						}
				}
				else//创建反L
					if (n == 4)
					{
						for (int i = 0; i < 4; i++)
							for (int j = 0; j < 4; j++)
							{
								if (j == 0 && i == 1 || j == 1 && i == 1 || j == 2 && i == 1 || j == 2 && i == 2)
									m_pSpritesNext[i][j]->setVisible(true);
								else
									m_pSpritesNext[i][j]->setVisible(false);
							}
					}
					else
						//创建卜
						if (n == 5)
						{
							for (int i = 0; i < 4; i++)
								for (int j = 0; j < 4; j++)
								{
									if (j == 1 && i == 1 || j == 1 && i == 0 || j == 1 && i == 2 || j == 2 && i == 1)
										m_pSpritesNext[i][j]->setVisible(true);
									else
										m_pSpritesNext[i][j]->setVisible(false);
								}
						}
						else
							//创建田
							if (n == 6)
							{
								for (int i = 0; i < 4; i++)
									for (int j = 0; j < 4; j++)
									{
										if (j == 1 && i == 1 || j == 2 && i == 1 || j == 1 && i == 2 || j == 2 && i == 2)
											m_pSpritesNext[i][j]->setVisible(true);
										else
											m_pSpritesNext[i][j]->setVisible(false);
									}
							}
}

bool GameBackground::createBoxOne()
{

	if (m_bArray[HEIGHT - 1][WIDTH / 2] && m_bArray[HEIGHT - 2][WIDTH / 2] && m_bArray[HEIGHT - 3][WIDTH / 2] && m_bArray[HEIGHT - 4][WIDTH / 2])
	{
		m_pSprites[HEIGHT - 3][WIDTH / 2]->setVisible(true);
		m_pSprites[HEIGHT - 4][WIDTH / 2]->setVisible(true);
		m_pSprites[HEIGHT - 2][WIDTH / 2]->setVisible(true);
		m_pSprites[HEIGHT - 1][WIDTH / 2]->setVisible(true);
		m_NodesArray[0].x = WIDTH / 2;
		m_NodesArray[0].y = HEIGHT - 4;
		m_NodesArray[1].x = WIDTH / 2;
		m_NodesArray[1].y = HEIGHT - 3;
		m_NodesArray[2].x = WIDTH / 2;
		m_NodesArray[2].y = HEIGHT - 2;
		m_NodesArray[3].x = WIDTH / 2;
		m_NodesArray[3].y = HEIGHT - 1;
		m_NodeRotate = m_NodesArray[1];
		return true;
	}
	return false;
}

bool GameBackground::createBoxTwo()
{
	if (m_bArray[HEIGHT - 2][WIDTH / 2 - 1] && m_bArray[HEIGHT - 2][WIDTH / 2] && m_bArray[HEIGHT - 1][WIDTH / 2] && m_bArray[HEIGHT - 1][WIDTH / 2 + 1])
	{
		m_pSprites[HEIGHT - 2][WIDTH / 2 - 1]->setVisible(true);
		m_pSprites[HEIGHT - 2][WIDTH / 2]->setVisible(true);
		m_pSprites[HEIGHT - 1][WIDTH / 2]->setVisible(true);
		m_pSprites[HEIGHT - 1][WIDTH / 2 + 1]->setVisible(true);
		m_NodesArray[0].x = WIDTH / 2 - 1;
		m_NodesArray[0].y = HEIGHT - 2;
		m_NodesArray[1].x = WIDTH / 2;
		m_NodesArray[1].y = HEIGHT - 2;
		m_NodesArray[2].x = WIDTH / 2;
		m_NodesArray[2].y = HEIGHT - 1;
		m_NodesArray[3].x = WIDTH / 2 + 1;
		m_NodesArray[3].y = HEIGHT - 1;
		m_NodeRotate = m_NodesArray[1];
		return true;
	}

	return false;
}

bool GameBackground::createBoxThree()
{
	if (m_bArray[HEIGHT - 1][WIDTH / 2 - 1] && m_bArray[HEIGHT - 1][WIDTH / 2] && m_bArray[HEIGHT - 2][WIDTH / 2] && m_bArray[HEIGHT - 2][WIDTH / 2 + 1])
	{
		m_pSprites[HEIGHT - 1][WIDTH / 2 - 1]->setVisible(true);
		m_pSprites[HEIGHT - 1][WIDTH / 2]->setVisible(true);
		m_pSprites[HEIGHT - 2][WIDTH / 2]->setVisible(true);
		m_pSprites[HEIGHT - 2][WIDTH / 2 + 1]->setVisible(true);
		m_NodesArray[0].x = WIDTH / 2 - 1;
		m_NodesArray[0].y = HEIGHT - 1;
		m_NodesArray[1].x = WIDTH / 2;
		m_NodesArray[1].y = HEIGHT - 2;
		m_NodesArray[2].x = WIDTH / 2;
		m_NodesArray[2].y = HEIGHT - 1;
		m_NodesArray[3].x = WIDTH / 2 + 1;
		m_NodesArray[3].y = HEIGHT - 2;
		m_NodeRotate = m_NodesArray[1];
		return true;
	}
	return false;
}

bool GameBackground::createBoxFour()
{
	if (m_bArray[HEIGHT - 1][WIDTH / 2 - 1] && m_bArray[HEIGHT - 2][WIDTH / 2 - 1] && m_bArray[HEIGHT - 2][WIDTH / 2] && m_bArray[HEIGHT - 2][WIDTH / 2 + 1])
	{
		m_pSprites[HEIGHT - 1][WIDTH / 2 - 1]->setVisible(true);
		m_pSprites[HEIGHT - 2][WIDTH / 2 - 1]->setVisible(true);
		m_pSprites[HEIGHT - 2][WIDTH / 2]->setVisible(true);
		m_pSprites[HEIGHT - 2][WIDTH / 2 + 1]->setVisible(true);
		m_NodesArray[0].x = WIDTH / 2 - 1;
		m_NodesArray[0].y = HEIGHT - 1;
		m_NodesArray[1].x = WIDTH / 2;
		m_NodesArray[1].y = HEIGHT - 2;
		m_NodesArray[2].x = WIDTH / 2 - 1;
		m_NodesArray[2].y = HEIGHT - 2;
		m_NodesArray[3].x = WIDTH / 2 + 1;
		m_NodesArray[3].y = HEIGHT - 2;
		m_NodeRotate = m_NodesArray[1];
		return true;
	}
	return false;
}

bool GameBackground::createBoxFive()
{
	if (m_bArray[HEIGHT - 2][WIDTH / 2 - 1] && m_bArray[HEIGHT - 2][WIDTH / 2] && m_bArray[HEIGHT - 2][WIDTH / 2 + 1] && m_bArray[HEIGHT - 1][WIDTH / 2 + 1])
	{
		m_pSprites[HEIGHT - 2][WIDTH / 2 - 1]->setVisible(true);
		m_pSprites[HEIGHT - 2][WIDTH / 2]->setVisible(true);
		m_pSprites[HEIGHT - 2][WIDTH / 2 + 1]->setVisible(true);
		m_pSprites[HEIGHT - 1][WIDTH / 2 + 1]->setVisible(true);
		m_NodesArray[0].x = WIDTH / 2 - 1;
		m_NodesArray[0].y = HEIGHT - 2;
		m_NodesArray[1].x = WIDTH / 2;
		m_NodesArray[1].y = HEIGHT - 2;
		m_NodesArray[2].x = WIDTH / 2 + 1;
		m_NodesArray[2].y = HEIGHT - 2;
		m_NodesArray[3].x = WIDTH / 2 + 1;
		m_NodesArray[3].y = HEIGHT - 1;
		m_NodeRotate = m_NodesArray[1];
		return true;
	}
	return false;
}

bool GameBackground::createBoxSix()
{
	if (m_bArray[HEIGHT - 1][7] && m_bArray[HEIGHT - 2][7] && m_bArray[HEIGHT - 3][7] && m_bArray[HEIGHT - 2][8])
	{
		m_pSprites[HEIGHT - 1][WIDTH / 2 - 1]->setVisible(true);
		m_pSprites[HEIGHT - 2][WIDTH / 2 - 1]->setVisible(true);
		m_pSprites[HEIGHT - 3][WIDTH / 2 - 1]->setVisible(true);
		m_pSprites[HEIGHT - 2][WIDTH / 2]->setVisible(true);
		m_NodesArray[0].x = WIDTH / 2 - 1;
		m_NodesArray[0].y = HEIGHT - 1;
		m_NodesArray[1].x = WIDTH / 2 - 1;
		m_NodesArray[1].y = HEIGHT - 2;
		m_NodesArray[2].x = WIDTH / 2 - 1;
		m_NodesArray[2].y = HEIGHT - 3;
		m_NodesArray[3].x = WIDTH / 2;
		m_NodesArray[3].y = HEIGHT - 2;
		m_NodeRotate = m_NodesArray[1];
		return true;
	}
	return false;
}

bool GameBackground::createBoxSeven()
{
	if (m_bArray[HEIGHT - 1][WIDTH / 2 - 1] && m_bArray[HEIGHT - 2][WIDTH / 2 - 1] && m_bArray[HEIGHT - 1][WIDTH / 2] && m_bArray[HEIGHT - 2][WIDTH / 2])
	{
		m_pSprites[HEIGHT - 1][WIDTH / 2 - 1]->setVisible(true);
		m_pSprites[HEIGHT - 2][WIDTH / 2 - 1]->setVisible(true);
		m_pSprites[HEIGHT - 1][WIDTH / 2]->setVisible(true);
		m_pSprites[HEIGHT - 2][WIDTH / 2]->setVisible(true);
		m_NodesArray[0].x = WIDTH / 2 - 1;
		m_NodesArray[0].y = HEIGHT - 1;
		m_NodesArray[1].x = WIDTH / 2 - 1;
		m_NodesArray[1].y = HEIGHT - 2;
		m_NodesArray[2].x = WIDTH / 2;
		m_NodesArray[2].y = HEIGHT - 1;
		m_NodesArray[3].x = WIDTH / 2;
		m_NodesArray[3].y = HEIGHT - 2;
		m_NodeRotate = m_NodesArray[1];
		return true;
	}
	return false;

}

void  GameBackground::reStart(Ref* pSender)
{
	cocos2d::CCDirector *director = cocos2d::CCDirector::sharedDirector();
	director->resume();
	m_isPaused = false;

	HelloWorld * parent = dynamic_cast<HelloWorld*>(this->getParent());
	parent->reStart();
	
}

void GameBackground::gameStart()
{

	srand(time(0));

	if (!m_bSuccess)
		return;
	if (m_nCurrentType == 0)
	{
		if (createBoxOne())
			//int random = CCRANDOM_0_1()*7;
		{
			int random = rand() % 7;
			m_nCurrentType = random;
			createNextBox(random);
			schedule(schedule_selector(GameBackground::moveDown), 0.5f);
		}
		else
		{
			gameOver();
		}
	}
	else
		if (m_nCurrentType == 1)
		{
			if (createBoxTwo())
			{
				int random = rand() % 7;
				m_nCurrentType = random;
				createNextBox(random);
				schedule(schedule_selector(GameBackground::moveDown), 0.5f);
			}
			else
			{
				gameOver();
			}
		}
		else
			if (m_nCurrentType == 2)
			{
				if (createBoxThree())
				{
					int random = rand() % 7;
					m_nCurrentType = random;
					createNextBox(random);
					schedule(schedule_selector(GameBackground::moveDown), 0.5f);
				}
				else
				{
					gameOver();
				}
			}
			else
				if (m_nCurrentType == 3)
				{
					if (createBoxFour())
					{
						int random = rand() % 7;
						m_nCurrentType = random;
						createNextBox(random);
						schedule(schedule_selector(GameBackground::moveDown), 0.5f);
					}
					else
					{
						gameOver();
					}
				}
				else
					if (m_nCurrentType == 4)
					{
						if (createBoxFive())
						{
							int random = rand() % 7;
							m_nCurrentType = random;
							createNextBox(random);
							schedule(schedule_selector(GameBackground::moveDown), 0.5f);
						}
						else
						{
							gameOver();
						}
					}
					else
						if (m_nCurrentType == 5)
						{
							if (createBoxSix())
							{
								int random = rand() % 7;
								createNextBox(random);
								m_nCurrentType = random;
								schedule(schedule_selector(GameBackground::moveDown), 0.5f);
							}
							else
							{
								gameOver();
							}
						}
						else
						{
							if (createBoxSeven())
							{
								int random = rand() % 7;
								m_nCurrentType = random;
								createNextBox(random);
								schedule(schedule_selector(GameBackground::moveDown), 0.5f);
							}
							else
							{
								gameOver();
							}
						}
	m_bControl = true;




	//cocos2d::CCLabelTTF* paihangTip = cocos2d::CCLabelTTF::create();
	//paihangTip->setString("Ranking List");
	//paihangTip->setFontSize(50);
	//addChild(paihangTip);
	//paihangTip->setPosition(cocos2d::CCPoint(145, 700));

	//m_score1Label = cocos2d::CCLabelTTF::create();
	////m_score1Label->setString(to_string(pCurrentScene->score1));
	//m_score1Label->setString("00");
	//m_score1Label->setFontSize(40);
	//addChild(m_score1Label);
	//m_score1Label->setPosition(cocos2d::CCPoint(145, 660));

	//m_score2Label = cocos2d::CCLabelTTF::create();
	////m_score2Label->setString(to_string(pCurrentScene->score2));
	//m_score2Label->setString("00");
	//m_score2Label->setFontSize(40);
	//addChild(m_score2Label);
	//m_score2Label->setPosition(cocos2d::CCPoint(145, 620));

	//m_score3Label = cocos2d::CCLabelTTF::create();
	////m_score3Label->setString(to_string(pCurrentScene->score3));
	//m_score3Label->setString("00");
	//m_score3Label->setFontSize(40);
	//addChild(m_score3Label);
	//m_score3Label->setPosition(cocos2d::CCPoint(145, 580));
}

bool GameBackground::isCurtainEnabled(NodePos position)
{
	int y = position.y;
	int x = position.x;
	
	if (y < 0 || y > HEIGHT - 1 || x < 0 || x > WIDTH - 1)
		return false;
	else
	{
		if (m_bArray[y][x])
			return true;
		else
			return false;
	}
}

void GameBackground::rotation(Ref* pSender)
{
	if (!m_bControl)
		return;
	for (int i = 0; i < 4; i++)
	{
		isMovedArray[i] = false;
	}
	int flag = 0;
	for (int m = 0; m < 4; m++)
	{
		NodePos nextPosition;
		int y = m_NodesArray[m].x - m_NodeRotate.x;
		int x = -(m_NodesArray[m].y - m_NodeRotate.y);
		nextPosition.x = x + m_NodeRotate.x;
		nextPosition.y = y + m_NodeRotate.y;
		if (!isCurtainEnabled(nextPosition))
			flag = 1;
	}
	if (flag == 0)
	{
		int num = 0;
		while (num != 4)
		{
			for (int k = 0; k < 4; k++)
			{

				if (!isMovedArray[k])
				{
					NodePos nextPosition;
					int y = m_NodesArray[k].x - m_NodeRotate.x;
					int x = -(m_NodesArray[k].y - m_NodeRotate.y);
					nextPosition.x = x + m_NodeRotate.x;
					nextPosition.y = y + m_NodeRotate.y;
					int flag2 = 0;
					for (int q = 0; q < 4; q++)
					{
						if (q != k)
						{
							if (m_NodesArray[q].x == nextPosition.x && m_NodesArray[q].y == nextPosition.y)
							{
								flag2 = 1;
							}
						}
					}
					if (flag2 == 0)
					{
						int j = m_NodesArray[k].x;
						int i = m_NodesArray[k].y;
						m_pSprites[i][j]->setVisible(false);
						int j2 = nextPosition.x;
						int i2 = nextPosition.y;
						m_pSprites[i2][j2]->setVisible(true);
						isMovedArray[k] = true;
						m_NodesArray[k].x = j2;
						m_NodesArray[k].y = i2;
						num++;
					}
				}
			}
		}
	}

}

void GameBackground::sortByLeft()
{

	NodePos mis;
	int k;
	bool b = false;
	for (int i = 0; i < 4; i++)
	{
		mis.x = m_NodesArray[i].x;
		mis.y = m_NodesArray[i].y;
		b = false;
		for (int j = i + 1; j < 4; j++)
		{
			if (m_NodesArray[j].x < mis.x)
			{
				mis.x = m_NodesArray[j].x;
				mis.y = m_NodesArray[j].y;
				k = j;
				b = true;
			}
		}
		if (b == true)
		{
			m_NodesArray[k].x = m_NodesArray[i].x;
			m_NodesArray[k].y = m_NodesArray[i].y;
			m_NodesArray[i].x = mis.x;
			m_NodesArray[i].y = mis.y;
		}
	}
}

void GameBackground::moveLeft(Ref* pSender)
{
	if (!m_bControl)
		return;
	int flag = 0;
	for (int m = 0; m < 4; m++)
	{
		NodePos nextPosition;
		nextPosition.x = m_NodesArray[m].x - 1;
		nextPosition.y = m_NodesArray[m].y;
		if (!isCurtainEnabled(nextPosition))
			flag = 1;
	}

	if (flag == 0)
	{
		sortByLeft();
		for (int k = 0; k < 4; k++)
		{

			int j = m_NodesArray[k].x;
			int i = m_NodesArray[k].y;
			m_pSprites[i][j]->setVisible(false);
			m_pSprites[i][j - 1]->setVisible(true);
			m_NodesArray[k].x -= 1;

		}
		m_NodeRotate.x -= 1;
	}
}

void  GameBackground::sortByRight()
{

	NodePos max;
	int k;
	bool b = false;
	for (int i = 0; i < 4; i++)
	{
		max.x = m_NodesArray[i].x;
		max.y = m_NodesArray[i].y;
		b = false;
		for (int j = i + 1; j < 4; j++)
		{
			if (m_NodesArray[j].x > max.x)
			{
				max.x = m_NodesArray[j].x;
				max.y = m_NodesArray[j].y;
				k = j;
				b = true;
			}
		}
		if (b == true)
		{
			m_NodesArray[k].x = m_NodesArray[i].x;
			m_NodesArray[k].y = m_NodesArray[i].y;
			m_NodesArray[i].x = max.x;
			m_NodesArray[i].y = max.y;
		}
	}
}

void GameBackground::moveRight(Ref* pSender)
{

	if (!m_bControl)
		return;
	int flag = 0;
	for (int m = 0; m < 4; m++)
	{
		NodePos nextPosition;
		nextPosition.x = m_NodesArray[m].x + 1;
		nextPosition.y = m_NodesArray[m].y;
		if (!isCurtainEnabled(nextPosition))
			flag = 1;
	}

	if (flag == 0)
	{
		sortByRight();
		for (int k = 0; k < 4; k++)
		{
			int j = m_NodesArray[k].x;
			int i = m_NodesArray[k].y;
			m_pSprites[i][j]->setVisible(false);
			m_pSprites[i][j + 1]->setVisible(true);
			m_NodesArray[k].x += 1;
		}
		m_NodeRotate.x += 1;
	}
}

void GameBackground::sortByDown()
{
	NodePos min;
	int k;
	bool b = false;
	for (int i = 0; i < 4; i++)
	{
		min.x = m_NodesArray[i].x;
		min.y = m_NodesArray[i].y;
		b = false;
		for (int j = i + 1; j < 4; j++)
		{
			if (m_NodesArray[j].y < min.y)
			{
				min.x = m_NodesArray[j].x;
				min.y = m_NodesArray[j].y;
				k = j;
				b = true;
			}
		}
		if (b == true)
		{
			m_NodesArray[k].x = m_NodesArray[i].x;
			m_NodesArray[k].y = m_NodesArray[i].y;
			m_NodesArray[i].x = min.x;
			m_NodesArray[i].y = min.y;
		}
	}
}

void GameBackground::moveDown(float t)
{
	if (!m_bControl)
		return;
	int flag = 0;
	for (int m = 0; m < 4; m++)
	{
		NodePos nextPosition;
		nextPosition.x = m_NodesArray[m].x;
		nextPosition.y = m_NodesArray[m].y - 1;
		if (!isCurtainEnabled(nextPosition))
			flag = 1;
	}

	if (flag == 0)
	{
		sortByDown();
		for (int k = 0; k < 4; k++)
		{
			int j = m_NodesArray[k].x;
			int i = m_NodesArray[k].y;
			m_pSprites[i][j]->setVisible(false);
			m_pSprites[i - 1][j]->setVisible(true);
			m_NodesArray[k].y -= 1;
		}
		m_NodeRotate.y -= 1;
	}
	else
	{
		finishdown();
	}
}


void GameBackground::accelerateDown(Ref* pSender)
{
	if (!m_bControl)
		return;
	int flag = 0;
	for (int m = 0; m < 4; m++)
	{
		NodePos nextPosition;
		nextPosition.x = m_NodesArray[m].x;
		nextPosition.y = m_NodesArray[m].y - 1;
		if (!isCurtainEnabled(nextPosition))
			flag = 1;
	}

	if (flag == 0)
	{
		sortByDown();
		for (int k = 0; k < 4; k++)
		{
			int j = m_NodesArray[k].x;
			int i = m_NodesArray[k].y;
			m_pSprites[i][j]->setVisible(false);
			m_pSprites[i - 1][j]->setVisible(true);
			m_NodesArray[k].y -= 1;
		}
		m_NodeRotate.y -= 1;
	}
	else
	{
		finishdown();
	}
}

void GameBackground::upDateCurtains()
{
	for (int i = 0; i < 4; i++)
	{
		int x = m_NodesArray[i].x;
		int y = m_NodesArray[i].y;
		m_bArray[y][x] = false;
	}
	for (int i = 0; i < HEIGHT; i++)
	{
		m_nCount = 0;
		for (int j = 0; j < WIDTH; j++)
		{
			if (!m_bArray[i][j])
				m_nCount += 1;
		}
		if (m_nCount == WIDTH)
		{

			for (int k = 0; k < WIDTH; k++)
			{
				m_pSprites[i][k]->setVisible(false);
				m_bArray[i][k] = true;
			}
			for (int m = i + 1; m < HEIGHT; m++)
			{
				for (int j2 = 0; j2 < WIDTH; j2++)
				{
					if (!m_bArray[m][j2])
					{
						m_pSprites[m][j2]->setVisible(false);
						m_bArray[m][j2] = true;
						m_pSprites[m - 1][j2]->setVisible(true);
						m_bArray[m - 1][j2] = false;
					}
				}
			}
			m_boom->setVisible(true);
			cocos2d::CCAnimation * boomAnimation = CCAnimation::create();
			boomAnimation->addSpriteFrameWithFileName("boom1.png");
			boomAnimation->addSpriteFrameWithFileName("boom2.png");
			boomAnimation->addSpriteFrameWithFileName("boom3.png");
			boomAnimation->addSpriteFrameWithFileName("boom4.png");
			boomAnimation->addSpriteFrameWithFileName("boom5.png");
			boomAnimation->setDelayPerUnit(0.2f);
			boomAnimation->setRestoreOriginalFrame(true);
			m_boom->runAction(CCSequence::create(CCAnimate::create(boomAnimation), CCCallFuncN::create(this, callfuncN_selector(GameBackground::hide)), NULL));
			//
			m_boom2->setVisible(true);
			cocos2d::CCAnimation * boomAnimation2 = CCAnimation::create();
			boomAnimation2->addSpriteFrameWithFileName("boom1.png");
			boomAnimation2->addSpriteFrameWithFileName("boom2.png");
			boomAnimation2->addSpriteFrameWithFileName("boom3.png");
			boomAnimation2->addSpriteFrameWithFileName("boom4.png");
			boomAnimation2->addSpriteFrameWithFileName("boom5.png");
			boomAnimation2->setDelayPerUnit(0.2f);
			boomAnimation2->setRestoreOriginalFrame(true);
			m_boom2->runAction(CCSequence::create(CCAnimate::create(boomAnimation2), CCCallFuncN::create(this, callfuncN_selector(GameBackground::hide)), NULL));
			i--;
			m_nScoreNum++;
		}
	}
}
void GameBackground::hide(Node*)
{
	m_boom->setVisible(false);
	m_boom2->setVisible(false);
}

void GameBackground::calculate()
{
	 score = m_nScoreNum * 10;
	char scores[20];
	std::stringstream stream;
	stream << score;
	stream >> scores;
	m_pscoreLabel->setString(scores);

}

void GameBackground::CountDown(string Count)
{

	m_ptimeLabel->setString(Count);

}



void GameBackground::finishdown()
{
	unschedule(schedule_selector(GameBackground::moveDown));
	upDateCurtains();
	calculate();
	gameStart();
}

void GameBackground::getBoxText(Ref* pSender)
{
	//char * str = (char *)this->mEditBox->getText();
	//editBoxLayer->getBoxText();

	
}

void GameBackground::gameOver()
{
	HelloWorld * parent = dynamic_cast<HelloWorld*>(this->getParent());
	parent->isgameover = true ;


	m_bSuccess = false;
	cocos2d::CCMenuItemImage * exitGame = cocos2d::CCMenuItemImage::create("gameover.png", "gameover.png", this, menu_selector(GameBackground::exitGame));
	//cocos2d::CCMenuItemImage * newGame = cocos2d::CCMenuItemImage::create("newgameA.png", "newgameB.png", this, menu_selector(GameBackground::reStart));
	//cocos2d::CCMenuItemImage * getBText = cocos2d::CCMenuItemImage::create("b2.png", "b2.png", this, menu_selector(GameBackground::getBoxText));
	
	//runEditBoxManager();
	
	HelloWorld* pCurrentScene = (HelloWorld*)CCDirector::getInstance()->getRunningScene();
	pCurrentScene->pscore = score;


	//editBoxLayer = new EditBoxManager;
	//editBoxLayer->autorelease();  //交给内存池进行管理  
	//pCurrentScene->addChild(editBoxLayer);


	exitGame->setScaleX(0.5);
	exitGame->setPositionY(-80);
	exitGame->setPositionX(-20);

	//newGame->setPositionY(20);
	//newGame->setPositionX(-20);
	//getBText->setScaleX(0.5);
	//getBText->setPositionY(-150);
	//getBText->setPositionX(0);
	m_pGameOverMenu = cocos2d::CCMenu::create(exitGame,NULL);
	addChild(m_pGameOverMenu);
	mscore1 = pCurrentScene->score1;
	mscore2 = pCurrentScene->score2;
	mscore3 = pCurrentScene->score3;
	//addChild(mEditBox);
	if (score >= mscore1)
	{
		mscore3 = mscore2;
		mscore2 = mscore1;
		mscore1 = score;
	}
	else if (score >= mscore2)
	{
		mscore3 = mscore2;
		mscore2 = score;
		
	}
	else if (score >= mscore3)
	{
		mscore3 = score;
	
	}



	//排行榜
	cocos2d::CCLabelTTF* paihangTip = cocos2d::CCLabelTTF::create();
	paihangTip->setString("Ranking List");
	paihangTip->setFontSize(50);
	addChild(paihangTip);
	paihangTip->setPosition(cocos2d::CCPoint(145, 700));
	
	m_score1Label = cocos2d::CCLabelTTF::create();
	m_score1Label->setString(to_string(mscore1));
	//m_score1Label->setString("0");
	m_score1Label->setFontSize(40);
	addChild(m_score1Label);
	m_score1Label->setPosition(cocos2d::CCPoint(145, 660));

	m_score2Label = cocos2d::CCLabelTTF::create();
	m_score2Label->setString(to_string(mscore2));
	//m_score2Label->setString("0");
	m_score2Label->setFontSize(40);
	addChild(m_score2Label);
	m_score2Label->setPosition(cocos2d::CCPoint(145, 620));

	m_score3Label = cocos2d::CCLabelTTF::create();
	m_score3Label->setString(to_string(mscore3));
	//m_score3Label->setString("0");
	m_score3Label->setFontSize(40);
	addChild(m_score3Label);
	m_score3Label->setPosition(cocos2d::CCPoint(145, 580));
	


}

void GameBackground::exitGame(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.", "Alert");
#else
	CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
#endif
}




