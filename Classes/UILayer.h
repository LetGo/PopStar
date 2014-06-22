#pragma once
#include "UseDefine.h"

class  UILayer :public Layer
{
public:
	 UILayer();
	~ UILayer();
	virtual bool init();
	void update(float dt);
	CREATE_FUNC(UILayer);
	void setCurrentScore(int score);
	void setCanGetScore(int score);
	void setUI(int level,int currentScore);
	void showClear();
	void blinkTarget();
	void step(float dt);
	void setLeaveStar(int leave);
private:
	LabelTTF* _levelLabel;
	LabelTTF* _targetScoreLabel;
	Label	* _currentScoreLabel;
	Label* _leaveStarLabel;
	LabelTTF* _canGetScoreLabel;
	bool _isShowCanGetScore;
	float _showTime;
	int _lastCurrentScore;
	int _currentScore;
	int _targetScore;
	bool _isShowClear;
	cocos2d::Sprite* _isStageClear;
};

