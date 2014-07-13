#pragma once
#include "UseDefine.h"

class Block;

class GameLayer :public Layer
{
public:
	GameLayer();
	~GameLayer();
	virtual bool init() override;
	virtual void update(float dt) override;
	virtual bool onTouchBegan(Touch *touch, Event *unused_event) override; 
	virtual void onTouchEnded(Touch *touch, Event *unused_event) override;
	CREATE_FUNC(GameLayer);
	void loadLevel(const unsigned int level,const bool isContinue);
private:
	void initStarMatrix(bool isContinue = false);
	void createAndDrapStart(const int row,const int col,const bool drap = true,const int index = -1);
	Point getPointOfBlock(int row,int col) const;
	Block *getStarByPos(const Point pos) const;
	void checkIfClearAll(float dt);
	void getStarList(Block* star,std::list<Block* > &starList);//递归获取连着的星星
	void getSelectStarList(Block* star);
	void actionForSelectStar();
	void tryRemoveStar(const Point location);
	void removeStar();
	void removeSelectSprite();
	void explodeStar(Block* star);
	void actionEndCallback(Node *node);
	void drapStar();
	void checkIsAnimationing();
	void starClearCallBack();
	void callBackExplodeStar(Node *node); //按队列消除回调
	void leaveStarCallBack(Node *node);//按队列消除不能再消除的
	void explodeleaveStar(Block* star);//执行消除剩余的星星动作特效
	void blinkLeaveStar(Node* node); //blink 按队列消除剩余的
	void checkCanContinue(Node* node);//游戏是否结束或者继续
	void checkMoveLeft(float dt);
	void move2Left(int startCol,int count);
	void playLandingEffect(Node *node);
	void enterNextLevel(Node *node);
	void showAwesome();
	void removeAwesome(Node *node);
	virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event) override;

	void saveStarData();
	void gameOver();
	void goMainScene(Node *node);

	void checkLeaveStar();
private:
	int _width;
	int _height;
	SpriteBatchNode* _batchNode;
	Block **_startArray;
	Point _leftBottomPos;
	bool _isTouchEnable;
	Block* _touchStar;
	bool _firstTouch; //第一次点击并且检测到有3个星星以上 设置为true
	bool _isAnimationing;
	bool _isNeedDrop;
	bool _isNeedMoveLeft;
	int _startCol;
	int _nullColCount;
	std::list<Block*> _starList;
	std::list<Sprite*> _selectList;
	std::list<Block*> _leaveStarList;
	int _leaveStar;
	bool _isClearAll;
	bool _isInitStarMatrix;
	float _exploreTime;
	bool _isCanExplore;
	bool _isGameOver;

	bool *_emptyCol;
};

//EOF

