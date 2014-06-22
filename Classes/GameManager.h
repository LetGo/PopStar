#pragma once
#include "UseDefine.h"


class UILayer;
class GameLayer;

class GameManager
{
public:
	GameManager();
	~GameManager();
	static GameManager* getInstance();
	cocos2d::Scene* startScene();
	void init();
	void UnInit();
	void startGame();
	void loadGameByLevel(const int level,const bool isContinue = false);
	void setCanGetScorre(int score);
	void setCurrentScore(int score);
	int getCurrentScore() const;
	void blinkTarget();
	void saveHightScore(int score);
	int getCurrentLevel() const;
	void backToMain();
	CC_SYNTHESIZE(bool,_stageClear,StageClear);
	void continueGame();
	void displayRank();
	void initDebugData();
	void setLeaveStar(int leave);
	void displaySaveLayer();
	CC_SYNTHESIZE(bool,_debug,Debug);
private:
	static GameManager* _instance ;
	GameLayer *_gameLayer;
	UILayer* _uiLayer;
	int _hightScore;
	int _currentLevel;
	int _currentScore;
};

