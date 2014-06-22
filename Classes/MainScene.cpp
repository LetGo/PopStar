#include "MainScene.h"
#include "GameManager.h"
#include "SoundBank.h"

#include "Rank.h"

#define BACKGROUND "background_layer0.png"
#define TITLE "title_popstar.png"
#define TIPS "tips.png"
#define HIGHT_SCORE "highscore.png"
#define MENU_START "menu_start.png"
#define MENU_RESUME "menu_resume.png"
#define MENU_RESUME_DISABLED "menu_resume_disabled.png"
#define MENU_SHOP "menu_shop.png"
#define LIGHT_RED "light_red.png"
#define LIGHT_BLUE "light_blue.png"
#define LIGHT_PINK "light_pink.png"
#define LIGHT_GREEN "light_green.png"
#define LIGHT_YELLOW "light_yellow.png"
const int TAG_STARTGAME = 100;
const int TAG_CONTINUEGAME = 200;
const int TAG_SHOWRANK = 300;
StartScene::StartScene()
{
}

StartScene::~StartScene()
{
	log("=====================EXIT StartScene");
}

Scene* StartScene::createScene(int hightScore){
	auto scene = Scene::create();
	auto layer = StartScene::create();
	layer->setHightScore(hightScore);
	scene->addChild(layer);
	return scene;
}

void StartScene::setHightScore(int score){
	char str[20];
	memset(str,0,20);
	sprintf(str,"%d",score);
	_hightScoreLabel->setString(str);
}

bool StartScene::init(){
	if (!Layer::init()){
		return false;
	}
	// background
	Size winSize = Director::getInstance()->getWinSize();
	auto background = Sprite::create(BACKGROUND);
	background->setAnchorPoint(Point(0.5f,1.0f));
	background->setPosition( Point(winSize.width/2, winSize.height) );
	this->addChild(background,-1);
	//title
	auto title = Sprite::create(TITLE);
	title->setPosition( Point(winSize.width/2, winSize.height/2 + 30) );
	this->addChild(title,1);
	//tips
	auto tips = Sprite::create(TIPS);
	tips->setPosition( Point(winSize.width/2, winSize.height/2 + 50 + 100) );
	this->addChild(tips,1);
	//highscore
	auto highscore = Sprite::create(HIGHT_SCORE);
	highscore->setPosition( Point(winSize.width/2, winSize.height- highscore->getContentSize().height/2) );
	this->addChild(highscore,1);

	TTFConfig config("fonts/tahoma.ttf",20);
	_hightScoreLabel = Label::createWithTTF(config,"0",TextHAlignment::LEFT);
	this->addChild(_hightScoreLabel,2);
	_hightScoreLabel->setPosition(winSize.width / 2 + 50,winSize.height - highscore->getContentSize().height / 2);

	//startgame
	auto startGame = MenuItemImage::create(
		MENU_START,
		MENU_START,
		CC_CALLBACK_1(StartScene::menuCallBack, this));
	startGame->setPosition( Point(winSize.width / 2, winSize.height / 2 - 80) );
	startGame->setTag(TAG_STARTGAME);
	//menu_resume
	MenuItemImage* menu_resume = MenuItemImage::create(
		MENU_RESUME,
		MENU_RESUME,
		MENU_RESUME_DISABLED,
		CC_CALLBACK_1(StartScene::menuCallBack, this));
	menu_resume->setPosition( Point(winSize.width / 2, startGame->getPositionY() - 40 ));
	menu_resume->setTag(TAG_CONTINUEGAME);
	if (USEDATA->getBoolForKey(CONTINUEGAME)){
		menu_resume->setEnabled(true);
	}
	else{
		menu_resume->setEnabled(false);
	}

	//menu_shop
	MenuItemImage* menu_shop = MenuItemImage::create(
		MENU_SHOP,
		MENU_SHOP,
		CC_CALLBACK_1(StartScene::menuCallBack, this));
	menu_shop->setPosition( Point(winSize.width / 2, menu_resume->getPositionY() - 40 ));
	menu_shop->setTag(TAG_SHOWRANK);
	auto menu2 = Menu::create(startGame,menu_resume, menu_shop,NULL);
	menu2->setPosition(Vec2::ZERO);
	this->addChild(menu2, 1);

	createLight();

	//android back_menu callback. win32 Esc callback
	//exit game
	auto listener = EventListenerKeyboard::create();
	listener->onKeyReleased = CC_CALLBACK_2(StartScene::onKeyReleased, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this); 


	//MenuItemToggle* toggle = MenuItemToggle::create();

	if (!AudioEngine->isBackgroundMusicPlaying()){
		SoundBank::getInstance()->playBackgound();
	}
	{
		MenuItemFont::setFontName("fonts/Marker Felt.ttf");
		MenuItemFont::setFontSize(20);
		auto title1 = MenuItemFont::create("Sound:");
		title1->setEnabled(false);
		title1->setPosition(Point(winSize.width - 80.0f,winSize.height - 40.0f));

		auto item1 = MenuItemToggle::createWithCallback( CC_CALLBACK_1(StartScene::onMusicCallBack, this),
			MenuItemFont::create( "Off"),
			MenuItemFont::create( "On" ),
			NULL );
		item1->setSelectedIndex(SoundBank::getInstance()->getEnable() ? 1 : 0);

		item1->setPosition(Point(winSize.width - 30.0f,winSize.height - 40.0f));
		auto menu = Menu::create(title1,item1, NULL);
		menu->setPosition(Vec2::ZERO);
		this->addChild(menu, 1);
	}
	{
		auto debug = MenuItemFont::create("Debug:");
		debug->setEnabled(false);
		debug->setPosition(Point(40.0f,winSize.height - 40.0f));

		auto item1 = MenuItemToggle::createWithCallback( CC_CALLBACK_1(StartScene::onDebugCallBack, this),
			MenuItemFont::create( "YES"),
			MenuItemFont::create( "NO" ),
			NULL );
		item1->setSelectedIndex(GameManager::getInstance()->getDebug() ? 0 : 1);

		item1->setPosition(Point(90.0f,winSize.height - 40.0f));
		auto menu = Menu::create(debug,item1, NULL);
		menu->setPosition(Vec2::ZERO);
		this->addChild(menu, 1);
	}
	return true;
}

void StartScene::onEnter(){
	Layer::onEnter();
	SoundBank::getInstance()->playLogo();
}

void StartScene::createLight(){
	Size winSize = Director::getInstance()->getWinSize();
	auto pinkLight = getLight(LIGHT_PINK,//filename
		Point(winSize.width - 100, winSize.height /2 - 20),//point
		0.5f,//delayTime
		1.5f,//fadeInTime
		0.4f,//delayTime2
		1.8f);//fadeOutTime

	auto redLight = getLight(LIGHT_RED,
		Point(40, winSize.height /2 + 70),0.0f,1.2f,0.4f,1.5f);

	auto blueLight = getLight(LIGHT_BLUE,
		Point(winSize.width - 40, winSize.height /2 + 30),0.7f,1.5f,0.3f,1.6f);

	auto greenLight = getLight(LIGHT_GREEN,Point(winSize.width/2 + 30, winSize.height /2 +70),
		0.1f,1.3f,0.3f,1.5f);

	auto yellowLight = getLight(LIGHT_YELLOW,
		Point( 70, winSize.height /2 - 30),1.0f,1.5f,0.17f,1.0f);
}

Sprite* StartScene::getLight(const std::string & name,
							 const Point & point,
							 const float delayTime,
							 const float fadeInTime,
							 const float delayTime2,
							 const float fadeOutTime){
	auto light = Sprite::create( name );
	light->setPosition( point );
	light->setScale(0.9f);
	this->addChild(light,0);
	light->runAction(CCRepeatForever::create(
		Sequence::create(
		DelayTime::create(delayTime),
		FadeIn::create(fadeInTime),
		DelayTime::create(delayTime2),
		FadeOut::create(fadeOutTime),
		NULL)));
	return light;
}

void StartScene::menuCallBack(cocos2d::Ref* pSender){
	SoundBank::getInstance()->playMenuStart();
	Blink * blink = Blink::create(0.5f,3);
	CallFuncN* callfunc = nullptr;
	MenuItemImage *item = (MenuItemImage*)pSender;
	if ( TAG_STARTGAME == item->getTag() ){
		callfunc =CallFuncN::create(CC_CALLBACK_1(StartScene::startNewGame, this));
	}else if ( TAG_CONTINUEGAME == item->getTag() ){
		callfunc = CallFuncN::create(CC_CALLBACK_1(StartScene::continueGame, this));
	}
	else if ( TAG_SHOWRANK == item->getTag() )
	{
		callfunc = CallFuncN::create(CC_CALLBACK_1(StartScene::onRank, this));
	}
	item->runAction(Sequence::create(blink,callfunc,NULL));
}

void StartScene::startNewGame(Node* node){
	GameManager::getInstance()->startGame();
 }

void StartScene::continueGame(Node* node){
	GameManager::getInstance()->continueGame();
}

void StartScene::onRank(Node* node){
	GameManager::getInstance()->displayRank();
}

void StartScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event){
	if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE){
		Director::getInstance()->end();
	}
}

void StartScene::onMusicCallBack(cocos2d::Ref* pSender){
	MenuItemToggle * toggle = dynamic_cast<MenuItemToggle*>(pSender);
	//log("toggle->getSelectedIndex() = %d",toggle->getSelectedIndex());
	if (toggle->getSelectedIndex() == 0)
	{
		SoundBank::getInstance()->stopPlayEffect();
		SoundBank::getInstance()->setEnable(false);
		USEDATA->setBoolForKey(SOUND_ENABEL,false);

	} 
	else
	{
		SoundBank::getInstance()->setEnable(true);
		USEDATA->setBoolForKey(SOUND_ENABEL,true);
	}
	
}

void StartScene::onDebugCallBack(cocos2d::Ref* pSender){
	MenuItemToggle * toggle = dynamic_cast<MenuItemToggle*>(pSender);
	if (toggle->getSelectedIndex() == 0)
	{
		USEDATA->setBoolForKey("debug",true);
		GameManager::getInstance()->setDebug(true);
		Director::getInstance()->setDisplayStats(true);
	} 
	else
	{
		USEDATA->setBoolForKey("debug",false);
		GameManager::getInstance()->setDebug(false);
		Director::getInstance()->setDisplayStats(false);
	}
}