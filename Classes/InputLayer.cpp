#include "InputLayer.h"
#include "Rank.h"
#include "GameManager.h"
#define FONT_NAME                       "fonts/tahoma.ttf"
#define FONT_SIZE                       18
#define TAG_CONFIRM						100
#define TAG_CANCEL						200

InputLayer::InputLayer()
{
	// Register Touch Event
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(InputLayer::onTouchBegan, this);
	listener->onTouchEnded = CC_CALLBACK_2(InputLayer::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithFixedPriority(listener, 10);
}

InputLayer::~InputLayer()
{
}

bool InputLayer::init(){
	if (!Layer::init())
	{
		return false;
	}
	auto s = Director::getInstance()->getWinSize();
	auto background = Sprite::create("input_bg.png");
	background->setPosition(Point(s.width/2,s.height/2));
	this->addChild(background);
	{
		TTFConfig config("fonts/tahoma.ttf",20);
		MenuItemImage* item = MenuItemImage::create("alert_button2.png","alert_button2_down.png",
			CC_CALLBACK_1(InputLayer::onConfirm, this));
		item->setScale(0.9f);
		item->setTag(TAG_CONFIRM);

		auto confirm = Label::createWithTTF(config,"CONFIRM",TextHAlignment::CENTER);
		confirm->setPosition(Point(item->getContentSize().width/2,item->getContentSize().height/2));
		item->addChild(confirm,2);

		MenuItemImage* item2 = MenuItemImage::create("alert_button1.png","alert_button1_down.png",
			CC_CALLBACK_1(InputLayer::onConfirm, this));
		item2->setScale(0.9f);
		auto cancel = Label::createWithTTF(config,"CANCEL",TextHAlignment::CENTER);
		cancel->setPosition(Point(item2->getContentSize().width/2,item2->getContentSize().height/2));
		item2->addChild(cancel,2);
		item2->setTag(TAG_CANCEL);
		auto menu = Menu::create(item2,item,NULL);
		menu->alignItemsHorizontallyWithPadding(10.0f);
		background->addChild(menu);
		menu->setPosition(background->getContentSize().width/2,40.0f);
	}

	 auto editBoxSize = Size(background->getContentSize().width - 80, 35);
	_editName = EditBox::create(editBoxSize, Scale9Sprite::create("green_edit.png"));
	_editName->setPosition(Vec2(s.width/2, s.height/2 + 40.0f));
	_editName->setFontName(FONT_NAME);
	_editName->setFontSize(FONT_SIZE);
	_editName->setFontColor(Color3B::RED);
	_editName->setPlaceHolder("type you name!");
	_editName->setPlaceholderFontColor(Color3B::WHITE);
	_editName->setMaxLength(8);
	_editName->setReturnType(EditBox::KeyboardReturnType::DONE);
	_editName->setDelegate(this);
	addChild(_editName);

	return true;
}


void InputLayer::editBoxEditingDidBegin(cocos2d::extension::EditBox* editBox){
	editBox->setText("");
}

void InputLayer::editBoxEditingDidEnd(cocos2d::extension::EditBox* editBox){
	//editBox->setText("type you name!");
}

void InputLayer::editBoxTextChanged(cocos2d::extension::EditBox* editBox, const std::string& text){
	 log("editBox %p TextChanged, text: %s ", editBox, text.c_str());
}

void InputLayer::editBoxReturn(cocos2d::extension::EditBox* editBox)
{
	//log("editBox %p TextChanged, text: %s ", editBox, editBox->getText());
}

void InputLayer::onConfirm(Ref* pSender){
	MenuItemImage* item = (MenuItemImage*)pSender;
	//log("text: %s ", _editName->getText());
	if (item->getTag() == TAG_CONFIRM)
	{
		Rank::getinstance()->save(_editName->getText(),GameManager::getInstance()->getCurrentScore());	
	}
	GameManager::getInstance()->backToMain();

}

//EOF
