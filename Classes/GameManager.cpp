#include "GameManager.h"
#include "GameLayer.h"
#include "MainScene.h"
#include "UILayer.h"
#include "SoundBank.h"
#include "TextBank.h"
#include "Rank.h"
#include "RankLayer.h"
#include "InputLayer.h"

GameManager* GameManager::_instance = nullptr;
// #ifndef M_DEBUG
// #define M_DEBUG
// #endif // !M_DEBUG

GameManager::GameManager()
	:_hightScore(0)
	,_currentLevel(1)
	,_currentScore(0)
	,_stageClear(false)
{

}

GameManager::~GameManager(){
	//UnInit();
	log("GameManager------ ");
}

GameManager* GameManager::getInstance(){
	if ( !_instance ){
		_instance = new GameManager();

// #ifdef M_DEBUG
// 		_instance->initDebugData();
// #endif // M_DEBUG

	}
	return _instance;
}

void GameManager::init(){
	if (!USEDATA->getIntegerForKey(HIGHTSCORE)){
		USEDATA->setIntegerForKey(HIGHTSCORE,0);
		USEDATA->flush();  
	}

	if (!USEDATA->getBoolForKey("debug"))
	{
		USEDATA->setBoolForKey("debug",false);
		USEDATA->flush(); 
	}
	_hightScore = USEDATA->getIntegerForKey(HIGHTSCORE);
	_debug = USEDATA->getBoolForKey("debug");
	SoundBank::getInstance()->init();
	TextBank::getInstance()->init();
	Rank::getinstance()->init();
}

void GameManager::UnInit()
{
	SoundBank::getInstance()->unInit();
	TextBank::getInstance()->unInit();
	Rank::getinstance()->unInit();
	if (_instance){
		delete _instance;
	}
}

Scene* GameManager::startScene(){
	_hightScore = USEDATA->getIntegerForKey(HIGHTSCORE);	
	auto scene = StartScene::createScene(_hightScore);
	return scene;
}

void GameManager::startGame(){
	Director* diector = Director::getInstance();
	auto scene = Scene::create();
	_gameLayer = GameLayer::create();
	_uiLayer = UILayer::create();
	loadGameByLevel(1);
	scene->addChild(_gameLayer);
	scene->addChild(_uiLayer,10);
	diector->replaceScene(scene);
}

void GameManager::loadGameByLevel(const int level,const bool isContinue ){
	_currentLevel = level;
	_stageClear = false;
	_gameLayer->loadLevel(level,isContinue);
	_uiLayer->setUI(level,_currentScore);
}

void GameManager::saveHightScore(int score){
	if ( score > _hightScore  ){
		USEDATA->setIntegerForKey(HIGHTSCORE,score);
	}
}
void GameManager::setCanGetScorre(int score){
	if (_uiLayer){
		_uiLayer->setCanGetScore(score);
	}
}

void GameManager::setCurrentScore(int score){
	if (_uiLayer){
		_currentScore += score;
		_uiLayer->setCurrentScore(_currentScore);
	}
}

int GameManager::getCurrentScore() const{
	return _currentScore;
}

void GameManager::blinkTarget(){
	if (_uiLayer){
		_uiLayer->blinkTarget();
	}
}

int GameManager::getCurrentLevel() const{
	return _currentLevel;
}

void GameManager::backToMain(){
	
	_currentScore = 0;
	Director* diector = Director::getInstance();
	_hightScore = UserDefault::getInstance()->getIntegerForKey("hightScore");
	auto scene = StartScene::createScene(_hightScore);
	diector->replaceScene(scene);
}

void GameManager::continueGame(){
	_currentLevel = USEDATA->getIntegerForKey(CURRENTLEVEL);
	_currentScore = USEDATA->getIntegerForKey(CURRENTSCORE);

	auto scene = Scene::create();
	_gameLayer = GameLayer::create();
	_uiLayer = UILayer::create();
	loadGameByLevel(_currentLevel,true);
	scene->addChild(_gameLayer);
	scene->addChild(_uiLayer,10);
	Director::getInstance()->replaceScene(scene);
}

void GameManager::displayRank(){
	auto scene = Scene::create();
	auto layer = RankLayer::create();
	scene->addChild(layer);
	Director::getInstance()->replaceScene(scene);
}

void GameManager::setLeaveStar(int leave){
	_uiLayer->setLeaveStar(leave);
}

void GameManager::displaySaveLayer(){
	auto input = InputLayer::create();
	_uiLayer->getParent()->addChild(input,3);
}

void GameManager::initDebugData(){
	USEDATA->setIntegerForKey("debug_9_0",1);
	USEDATA->setIntegerForKey("debug_8_0",1);
	USEDATA->setIntegerForKey("debug_7_0",1);
	USEDATA->setIntegerForKey("debug_6_0",1);
	USEDATA->setIntegerForKey("debug_5_0",2);
	USEDATA->setIntegerForKey("debug_4_0",2);
	USEDATA->setIntegerForKey("debug_3_0",1);
	USEDATA->setIntegerForKey("debug_2_0",1);
	USEDATA->setIntegerForKey("debug_1_0",1);
	USEDATA->setIntegerForKey("debug_0_0",1);

	USEDATA->setIntegerForKey("debug_9_1",3);
	USEDATA->setIntegerForKey("debug_8_1",3);
	USEDATA->setIntegerForKey("debug_7_1",3);
	USEDATA->setIntegerForKey("debug_6_1",3);
	USEDATA->setIntegerForKey("debug_5_1",4);
	USEDATA->setIntegerForKey("debug_4_1",4);
	USEDATA->setIntegerForKey("debug_3_1",3);
	USEDATA->setIntegerForKey("debug_2_1",2);
	USEDATA->setIntegerForKey("debug_1_1",4);
	USEDATA->setIntegerForKey("debug_0_1",1);

	USEDATA->setIntegerForKey("debug_9_2",1);
	USEDATA->setIntegerForKey("debug_8_2",1);
	USEDATA->setIntegerForKey("debug_7_2",1);
	USEDATA->setIntegerForKey("debug_6_2",1);
	USEDATA->setIntegerForKey("debug_5_2",2);
	USEDATA->setIntegerForKey("debug_4_2",2);
	USEDATA->setIntegerForKey("debug_3_2",3);
	USEDATA->setIntegerForKey("debug_2_2",3);
	USEDATA->setIntegerForKey("debug_1_2",1);
	USEDATA->setIntegerForKey("debug_0_2",1);

	USEDATA->flush();
}