#pragma once
#include "UseDefine.h"

class StartScene :public Layer
{
public:
	StartScene();
	~StartScene();
	static Scene* createScene(int hightScore);
	virtual bool init();
	virtual void onEnter();
	CREATE_FUNC(StartScene);
	void menuCallBack(cocos2d::Ref* pSender);
	void startNewGame(Node* node);
	void continueGame(Node* node);
	void onRank(Node* node);
	void createLight();
	Sprite* getLight(const std::string & name,const Point & point,const float delayTime,const float fadeInTime,const float delayTime2,const float fadeOutTime);
	void setHightScore(int score);
	virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
	void onMusicCallBack(cocos2d::Ref* pSender);
	void onDebugCallBack(cocos2d::Ref* pSender);
private:
	Label* _hightScoreLabel;
};

