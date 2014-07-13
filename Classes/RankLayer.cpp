#include "RankLayer.h"
#include "Rank.h"
#include "GameManager.h"

RankLayer::RankLayer()
{
}

RankLayer::~RankLayer()
{
}

bool RankLayer::init(){
	if (!Layer::init()){
		return false;
	}

	// background
	Size winSize = Director::getInstance()->getWinSize();
	auto background = Sprite::create("background_layer0.png");
	background->setAnchorPoint(Point(0.5f,1.0f));
	background->setPosition( Point(winSize.width/2, winSize.height) );
	this->addChild(background);

	TTFConfig config("fonts/tahoma.ttf",20);
	//title
	auto gitHttp = Label::createWithTTF(config,"Ranking List",TextHAlignment::CENTER);
	gitHttp->setPosition(Point(winSize.width/2,winSize.height - 20.0f));
	this->addChild(gitHttp,3);
	gitHttp->setColor(Color3B::YELLOW);

	// Create the list view ex
	listView = ListView::create();
	// set list view ex direction
	listView->setDirection(ui::ScrollView::Direction::VERTICAL);
	listView->setTouchEnabled(true);
	listView->setBounceEnabled(true);
	listView->setBackGroundImage("green_edit.png");
	listView->setBackGroundImageScale9Enabled(true);
	listView->setSize(Size(240, 400));
	listView->setPosition(Point(40.0f,40.0f));
	listView->addEventListener(CC_CALLBACK_2(RankLayer::selectedItemEvent, this));
	this->addChild(listView);

	// create model
	Button* default_button = Button::create("input.png", "input.png");
	default_button->setScale9Enabled(true);
	default_button->setSize(Size(240.0f,45.0f));
	default_button->setName("Title Button");
	default_button->setTitleFontSize(25);
	
	Layout* default_item = Layout::create();
	default_item->setTouchEnabled(true);
	default_item->setSize(default_button->getSize());
	default_button->setPosition(Vec2(default_item->getSize().width / 2.0f,
		default_item->getSize().height / 2.0f));
	default_item->addChild(default_button);
	default_item->setPosition(Point(20.0f,10.0f));
	// set model
	listView->setItemModel(default_item);	setRank();
	listView->setGravity(ListView::Gravity::CENTER_VERTICAL);

	// set items margin
	listView->setItemsMargin(5.0f);

	//exit game
	auto listener = EventListenerKeyboard::create();
	listener->onKeyReleased = CC_CALLBACK_2(RankLayer::onKeyReleased, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this); 
	return true;
}

void RankLayer::setRank(){
	listView->removeAllItems();
	std::list<info*> infoList = Rank::getinstance()->getList();
	for (int i = 0; i < infoList.size(); ++i){
		listView->pushBackDefaultItem();
	}

	int i = 0;
	char str[30] = {0};
	for (auto itr = infoList.cbegin(); itr != infoList.cend(); itr++,++i){
		info* temp = (info*)*itr;
		Widget* item = listView->getItem(i);
		Button* button = static_cast<Button*>(item->getChildByName("Title Button"));
		ssize_t index = listView->getIndex(item);
		sprintf(str," %d: %s %d",i+1,temp->name.c_str(),temp->score);
		button->setTitleText(str);
	}

}

void RankLayer::selectedItemEvent(Ref *pSender, ListView::EventType type){
	switch (type)
	{
	case cocos2d::ui::ListView::EventType::ON_SELECTED_ITEM_START:
		{
			ListView* listView = static_cast<ListView*>(pSender);
			CC_UNUSED_PARAM(listView);
			//CCLOG("select child start index = %ld", listView->getCurSelectedIndex());
			break;
		}
	case cocos2d::ui::ListView::EventType::ON_SELECTED_ITEM_END:
		{
			ListView* listView = static_cast<ListView*>(pSender);
			CC_UNUSED_PARAM(listView);
			//CCLOG("select child end index = %ld", listView->getCurSelectedIndex());
			break;
		}
	default:
		break;
	}
}

void RankLayer::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event){
	if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE){
		GameManager::getInstance()->backToMain();
	}
}

//EOF