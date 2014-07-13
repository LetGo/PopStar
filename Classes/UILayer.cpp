#include "UILayer.h"
#include "GameManager.h"
#include "SoundBank.h"
#include "TextBank.h"

#include <math.h>

UILayer:: UILayer():
	_showTime(0.0f)
	,_isShowCanGetScore(false)
	,_currentScore(0)
	,_targetScore(0)
	,_isShowClear(false)
	,_stageClearSprite(NULL)
	,_lastCurrentScore(-1)
{
}

UILayer::~ UILayer()
{
	log("=====================EXIT UILayer");
}

 bool UILayer::init(){
	 if (!Layer::init()){
		 return false;
	 }

	 Size winSize = Director::getInstance()->getWinSize();

	_levelLabel = LabelTTF::create(TextBank::getInstance()->getLevelText(),"Arial",20);
	_levelLabel->setAnchorPoint(Point(0,0));
	_levelLabel->setPosition(10,winSize.height-30);
	this->addChild(_levelLabel);

	_targetScoreLabel = LabelTTF::create(TextBank::getInstance()->getTargetText(), "Arial", 20);
	_targetScoreLabel->setAnchorPoint(Point(0,0));
	_targetScoreLabel->setPosition(100,winSize.height-30);
	this->addChild(_targetScoreLabel);

	 _currentScoreLabel = Label::createWithCharMap("fonts/tuffy_bold_italic-charmap.plist");
	 _currentScoreLabel->setPosition(winSize.width/2,winSize.height-40);
	 _currentScoreLabel->setColor(Color3B::YELLOW);
	 this->addChild(_currentScoreLabel);
	 
	 _leaveStarLabel = Label::createWithCharMap("fonts/tuffy_bold_italic-charmap.plist");
	 _leaveStarLabel->setPosition(winSize.width/2 - 100.0f,winSize.height- 40.0f);
	 _leaveStarLabel->setColor(Color3B::GREEN);
	 _leaveStarLabel->setScale(0.8f);
	 this->addChild(_leaveStarLabel);
	_leaveStarLabel->setVisible(GameManager::getInstance()->getDebug());

	 _canGetScoreLabel = LabelTTF::create("", "Arial", 18);
	 _canGetScoreLabel->setPosition(winSize.width/2,winSize.height-90);
	 this->addChild(_canGetScoreLabel);
	 schedule( schedule_selector(UILayer::step) );
	 scheduleUpdate();
	 return true;
 }

 void UILayer::setUI(int level,int currentScore){
	 char str[10] ;
	 memset(str,0,10);
	 std::sprintf(str,"%d",level);
	 std::string levelstr = TextBank::getInstance()->getLevelText();
	 _levelLabel->setString(levelstr + str);

	 std::sprintf(str,"%d",currentScore);
	 _currentScoreLabel->setString(str);


	 if (level == 1)
	 {
		 _targetScore = 1000;
	 }else if ( level == 2)
	 {
		 _targetScore = 3000;
	 }
	 else if ( level == 3)
	 {
		 _targetScore = 5000;
	 }
	 else if ( level == 4)
	 {
		 _targetScore = 8000;
	 }
	 else 
	 {
		 _targetScore = 8000 + (level - 4) * 2000;
	 }
	 std::sprintf(str,"%d",_targetScore);
	 
	 _targetScoreLabel->setString( TextBank::getInstance()->getTargetText()+ str);

	 _lastCurrentScore = currentScore;
	 Size winSize = Director::getInstance()->getWinSize();
	 if (_targetScore < currentScore && _stageClearSprite == NULL)
	 {
		 _isShowClear = true;
		 _stageClearSprite = Sprite::create("stage_clear.png");
		 _stageClearSprite->setPosition(Point(winSize.width-50,winSize.height-80));
		 _stageClearSprite->setScale(0.35f);
		 this->addChild(_stageClearSprite);
	 }
	 else{
		 _isShowClear = false;
		 if (_stageClearSprite != NULL)
		 {
			 _stageClearSprite->setScale(1);
			 _stageClearSprite->setVisible(false);
		 }
	 }

 }

 void UILayer::setCurrentScore(int score){
	 char str[30] ;
	 memset(str,0,30);
	 _currentScore = score;
	 std::sprintf(str,"%d",score);
	 _currentScoreLabel->setString(str);
	 GameManager::getInstance()->saveHightScore(score);
 }

 void UILayer::setCanGetScore(int score){
	 char str[40] ;
	 memset(str,0,40);
	 int num = sqrt( score / 5 );

	 std::sprintf(str,TextBank::getInstance()->getCanGetScoreText().c_str(),num,score);
	 _canGetScoreLabel->setString(str);
	 _canGetScoreLabel->setScale(0);
	 _canGetScoreLabel->runAction(Spawn::create(ScaleTo::create(0.25f,1),FadeIn::create(0.25f),NULL));
	 _isShowCanGetScore = true;
	 _showTime = 0.0f;
 }

 void UILayer::update(float dt){
	 if (_isShowCanGetScore)
	 {
		 _showTime += dt;
		// CCLog("%02f",_showTime);
		 if (_showTime > 3.5f)
		 {
			 _showTime = 0.0f;			 
			 _canGetScoreLabel->runAction(FadeOut::create(0.5f));
			 _isShowCanGetScore = false;
			 //_canGetScoreLabel->setString("");
		 }
	 }

	 if ( _currentScore >= _targetScore && !_isShowClear)
	 {
		 GameManager::getInstance()->setStageClear(true);
		 _isShowClear = true;
		 showClear();
	 }
 }

 void UILayer::showClear(){
	 SoundBank::getInstance()->playClear();
	 Size winSize = Director::getInstance()->getWinSize();
	 if (_stageClearSprite == NULL)
	 {
		 _stageClearSprite = Sprite::create("stage_clear.png");
		 
		 this->addChild(_stageClearSprite);
	 }
	 _stageClearSprite->setVisible(true);
	 _stageClearSprite->setPosition(winSize.width/2,winSize.height/2);
	 _stageClearSprite->runAction(Sequence::create(
		 Blink::create(0.4f,3),
		 DelayTime::create(0.3f),
		 Spawn::create(ScaleTo::create(0.2f,0.35f),MoveTo::create(0.2f,Point(winSize.width-50,winSize.height-80)),NULL),
		 NULL
		 ));
	 blinkTarget();
 }

 void UILayer::blinkTarget(){
	 _targetScoreLabel->runAction(Sequence::create( DelayTime::create(1.0f),Blink::create(1.0f,4),NULL));
 }

 void UILayer::step(float dt){
	 char str[30] ;
	 memset(str,0,30);
	 if (_lastCurrentScore < _currentScore){ 
		 if(_currentScore - _lastCurrentScore > 50 ){
			 _lastCurrentScore += 2;
		 }else{
			 _lastCurrentScore += 1;
		 }

		 if (_lastCurrentScore > _currentScore){
			 _lastCurrentScore = _currentScore;
		 }
		 std::sprintf(str,"%d",_lastCurrentScore);
		 _currentScoreLabel->setString(str);
	 }
	 //	 std::sprintf(str,"%d",score);
 }

 void UILayer::setLeaveStar(int leave){
	 char str[10] ;
	 memset(str,0,10);
	 std::sprintf(str,"%d",leave);
	 _leaveStarLabel->setString(str);
 }

 //EOF