#include "GameLayer.h"
#include "Block.h"
#include "GameManager.h"
#include "SoundBank.h"
#include "Rank.h"

const std::string BACKGROUND = "background_layer0.png";
const std::string STAR_PLIST = "block.plist";
const std::string STAR_PVR = "block.pvr.ccz";
const std::string STAR_SELECT = "block_select.png";
const std::string COMBO_AWESOME = "combo_awesome.png";
const std::string PATICLE_NAME = "paticle_%d.plist";
const float START_GAP = 0.4f;

GameLayer::GameLayer():
	_height(0)
	,_width(0)
	,_isTouchEnable(false)
	,_batchNode(nullptr)
	,_startArray(NULL)
	,_touchStar(NULL)
	,_firstTouch(false)
	,_isAnimationing(true)
	,_isNeedDrop(false)
	,_isNeedMoveLeft(false)
	,_startCol(-1)
	,_nullColCount(0)
	,_leaveStar(0)
	,_isClearAll(false)
	,_isInitStarMatrix(false)
	,_isGameOver(false)
{
}

GameLayer::~GameLayer()
{
	_selectList.clear();
	_starList.clear();
	_leaveStarList.clear();
	_startArray = nullptr;
	_batchNode->removeAllChildrenWithCleanup(true);
	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile(STAR_PLIST);
	log("=====================EXIT GameLayer");
}

bool GameLayer::init(){
	if (!Layer::init()){
		return false;
	}
	// background
	Size winSize = Director::getInstance()->getWinSize();
	auto background = Sprite::create(BACKGROUND);
	background->setAnchorPoint(Point(0.5f,1.0f));
	background->setPosition( Point(winSize.width/2, winSize.height) );
	this->addChild(background,-1);

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile(STAR_PLIST);
	_batchNode = SpriteBatchNode::create(STAR_PVR);
	addChild(_batchNode);

	_width = 10;
	_height = 10;

	float x = (winSize.width - _width * Block::getContentWidth() - (_width - 1) * START_GAP) / 2;
	float y = 1.0f;
	_leftBottomPos.setPoint(x,y);

	//Touch Event
	auto touchListener = EventListenerTouchOneByOne::create();
 	touchListener->onTouchBegan = CC_CALLBACK_2(GameLayer::onTouchBegan, this);
 	touchListener->onTouchEnded = CC_CALLBACK_2(GameLayer::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
	
	auto listener = EventListenerKeyboard::create();
	listener->onKeyReleased = CC_CALLBACK_2(GameLayer::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this); 

	return true;
}

void GameLayer::loadLevel(const unsigned int level,const bool isContinue){
	_leaveStar = 0;
	_isAnimationing = true;
	_isNeedDrop = false;
	_isNeedMoveLeft = false;
	_isClearAll = false;
	_emptyCol = (bool*)malloc(sizeof(bool) * _width);
	memset((void *)_emptyCol, 0, sizeof(bool) * _width);
	GameManager::getInstance()->setLeaveStar(_width * _height);
	scheduleUpdate();
	schedule(schedule_selector(GameLayer::checkIfClearAll),1.5f);
	schedule(schedule_selector(GameLayer::checkMoveLeft),0.30f);
	//schedule(schedule_selector(GameLayer::checkLeaveStar),1.5f);
	initStarMatrix(isContinue);
}

void GameLayer::initStarMatrix(bool isContinue){
	if (!_isInitStarMatrix){
		int arraySize = sizeof(Block*) * _height * _width;
		_startArray = (Block**)malloc(arraySize);
		memset((void*)_startArray,0,arraySize);
		_isInitStarMatrix = true;
	}

	if (isContinue){
		char key[30];
		memset(key,0,30);
		for (int row = 0; row < _height; ++row){//行
			for(int col = 0; col < _width ;++col){//列
				sprintf(key,STAR_INFO,row,col);
				int index = USEDATA->getIntegerForKey(key);
				if (index != -1){
					createAndDrapStart(row,col,false,index); 
				}else{
					_startArray[row * _width + col] = NULL;
				}
			}
		}
	}else{
		for (int row = 0;row<_height; ++row){
			for(int col =0; col < _width; ++col){
				createAndDrapStart(row,col); 
			}
		}
	}
	if (!isContinue)
	{
		CallFuncN * callfunc = CallFuncN::create(CC_CALLBACK_1(GameLayer::playLandingEffect, this));
		DelayTime* dt = DelayTime::create(0.15f);
		DelayTime* fdt = DelayTime::create(0.4f);
		this->runAction(Sequence::create(fdt,callfunc,dt,
			callfunc->clone(),dt->clone(),
			callfunc->clone(),dt->clone(),
			callfunc->clone(),dt->clone(),
			callfunc->clone(),dt->clone(),
			callfunc->clone(),dt->clone(),
			NULL));
	}


	_isTouchEnable = true;
	GameManager::getInstance()->blinkTarget();
}

void GameLayer::update(float dt){
	checkIsAnimationing();
}

void GameLayer::checkIsAnimationing(){
	if (_isAnimationing) {
		_isAnimationing = false;
		for (int i = 0; i < _height * _width; i++) {
			Block *star = _startArray[i];
			if (star && star->getNumberOfRunningActions() > 0) {
				_isAnimationing = true;
				break;
			}
		}
	}
	_isTouchEnable = !_isAnimationing;

	if (!_isAnimationing) {
		if (_isNeedDrop) {
			drapStar();
			_isTouchEnable = false;
			_isNeedDrop = false;
		}

		if (_isNeedMoveLeft){
			_isTouchEnable = false;
			move2Left(_startCol,_nullColCount);
			_isNeedMoveLeft = false;		
		}

		if ( _isClearAll ){
			_isTouchEnable = false;
			starClearCallBack();
		}
	}
}

void GameLayer::starClearCallBack(){
	unscheduleUpdate();
	unschedule(schedule_selector(GameLayer::checkIfClearAll));
	unschedule(schedule_selector(GameLayer::checkMoveLeft));
	//unschedule(schedule_selector(GameLayer::checkLeaveStar));
	Vector<FiniteTimeAction*> arrayOfActions;
	for (int i = 0; i < (int)_leaveStarList.size();++i){
		if (i < 10){
			FiniteTimeAction *dt = DelayTime::create(0.15f);
			FiniteTimeAction *cf = CallFuncN::create(CC_CALLBACK_1(GameLayer::callBackExplodeStar, this));
			arrayOfActions.pushBack(dt);
			arrayOfActions.pushBack(cf);
		}else{
			break;
		}
	}	

	FiniteTimeAction *dt = DelayTime::create(0.11f);
	FiniteTimeAction *blinkLeaveStar = CallFuncN::create(CC_CALLBACK_1(GameLayer::blinkLeaveStar, this)) ;
	FiniteTimeAction *dt2 = DelayTime::create(0.30f);
	FiniteTimeAction *cf = CallFuncN::create(CC_CALLBACK_1(GameLayer::leaveStarCallBack, this)) ;
	FiniteTimeAction *dt3 = DelayTime::create(0.5f);
	FiniteTimeAction *checkCanContinue = CallFuncN::create(CC_CALLBACK_1(GameLayer::checkCanContinue, this)) ;
	arrayOfActions.pushBack( dt );
	arrayOfActions.pushBack( blinkLeaveStar );
	arrayOfActions.pushBack( dt2 );
	arrayOfActions.pushBack( cf );
	arrayOfActions.pushBack( dt3 );
	arrayOfActions.pushBack( checkCanContinue );
	log("arrayOfActions  = %d ,_leaveStarList = %d",(int)(arrayOfActions.size() - 6),(int)_leaveStarList.size());

	this->runAction(Sequence::create(arrayOfActions));
}

void GameLayer::callBackExplodeStar(Node *node){

	for (auto itr = _leaveStarList.cbegin();  itr != _leaveStarList.cend() ;  itr++){
		Block* star = (Block*)*itr;
		if (star){
			log("pop : %d",star->getRow() * _width + star->getCol());
			explodeleaveStar(star);
			//remove from list
			_leaveStarList.remove(star);
			log("leave : %d",_leaveStarList.size());
			break;
		}
	}
}

void GameLayer::blinkLeaveStar(Node* node){
	if (_leaveStarList.size() <= 0){
		return;
	}
	float time = 0.25f;
	for (auto itr = _leaveStarList.cbegin();  itr != _leaveStarList.cend() ; ++itr){
		Block* star = (Block*)(*itr);
		if (star){		
			star->runAction(Blink::create(time,3));		
		}	
	}
}

void GameLayer::leaveStarCallBack(Node *node){
	if (_leaveStarList.size() <= 0){
		return;
	}
	log("_leaveStarList = %d",_leaveStarList.size());
	static int leave = 2;
	float time = 0.1f;
	int all = _leaveStarList.size();
	int gap = 1;
	if (all > 10 && all < 15){
		gap = 2;
	}else if(all >= 15 && all < 20 ){
		gap = 3;
	}
	else{
		gap = 4;
	}
	for (auto itr = _leaveStarList.cbegin();  itr != _leaveStarList.cend() ; ++itr){
		Block* star = (Block*)(*itr);
		if (star){
			if (leave++ % gap == 0)
			{
				explodeleaveStar(star);			
			}
			else{
				star->runAction(Sequence::create(
					ScaleTo::create(time, 0.0f),
					CallFuncN::create(CC_CALLBACK_1(GameLayer::actionEndCallback, this)),
					NULL));
			}
		}
	}

}

void GameLayer::explodeleaveStar(Block* star){
	SoundBank::getInstance()->playPopStar();
	float time = 0.1f;
	star->runAction(Sequence::create(
		ScaleTo::create(time, 0.0f),
		CallFuncN::create(CC_CALLBACK_1(GameLayer::actionEndCallback, this)),
		NULL));
	char fileName[30];
	memset(fileName,0,30);
	sprintf(fileName,PATICLE_NAME.c_str(),star->getIndex());
	auto particleStars = ParticleSystemQuad::create(fileName);
	particleStars->setAutoRemoveOnFinish(true);
	particleStars->setBlendAdditive(false);
	particleStars->setPosition(star->getPosition());
	addChild(particleStars, 20);
}

void GameLayer::checkCanContinue(Node* node){
	if ( GameManager::getInstance()->getStageClear() ){
		//NEXT LEVEL
		SoundBank::getInstance()->playCheers();
		this->runAction(Sequence::create(DelayTime::create(2.3f),
			CallFuncN::create(CC_CALLBACK_1(GameLayer::enterNextLevel, this)),NULL));
	}else
	{
		_isGameOver = true;
		saveStarData();
		gameOver();
		log("GAME OVER !!!!!!!!!");
	}
}

void GameLayer::enterNextLevel(Node *node){
	GameManager::getInstance()->loadGameByLevel(GameManager::getInstance()->getCurrentLevel() + 1,false);
}

void GameLayer::createAndDrapStart(const int row,const int col,const bool drap,const int index){
	Size winSize = Director::getInstance()->getWinSize();
	Point endPos = getPointOfBlock(row,col);
	Point startPos = Point(endPos.x,endPos.y + winSize.height);
	Block *star = nullptr;
	if (drap && index < 0){
		star = Block::create(row,col);
		star->setPosition(startPos);
		float dautime = startPos.y / (1.5f * winSize.height);
		MoveTo* move = MoveTo::create(dautime,endPos);
		star->runAction(move);	
	}else if(!drap && index >= 0){
		star = Block::createByIndex(row,col,index);
		star->setPosition(endPos);
	}
	// must add to _batchNode
	_batchNode->addChild(star);
	_startArray[row * _width + col] = star;
}

Point GameLayer::getPointOfBlock(int row,int col) const{
	float x = _leftBottomPos.x + Block::getContentWidth() / 2 + (Block::getContentWidth() + START_GAP) * col;
	float y = _leftBottomPos.y + Block::getContentWidth() / 2 + (Block::getContentWidth() + START_GAP) * row;
	return Point(x,y);
}

Block* GameLayer::getStarByPos(const Point pos) const {
	Block * star = nullptr;
	Rect starRect = Rect(0,0,0,0);
	for (int i =0 ; i < _width * _height ; ++i){
		star = _startArray[i];
		if (star){
			starRect.origin.x = star->getPositionX() - Block::getContentWidth() / 2 ;
			starRect.origin.y = star->getPositionY() - Block::getContentWidth() / 2 ;
			starRect.size = star->getContentSize();
			if (starRect.containsPoint(pos)){
				return star;
			}
		}
	}
	return nullptr;
}

bool GameLayer::onTouchBegan(Touch *touch, Event *unused_event){
	if(!_isTouchEnable || _isNeedMoveLeft || _isNeedDrop || _isGameOver)
		return false;
	Point location = touch->getLocation();
	_touchStar = getStarByPos(location);

	if ( _touchStar ) {
		if ( !_firstTouch ){
			//第一次点击 选择可以消除的星星
			getSelectStarList(_touchStar);		
		}else{
			//第二次点击 1消除 2再次选择可以消除的星星
			tryRemoveStar(location);
		}
	}	
	return true;
}

void GameLayer::onTouchEnded(Touch *touch, Event *unused_event)
{
	if (_touchStar != NULL){
		_touchStar->toString();
		_touchStar = NULL;
	}	
}

void GameLayer::checkIfClearAll(float dt){
	if (_isAnimationing || _isNeedDrop || _isNeedMoveLeft || _isClearAll){
		return;
	}
	Block* star = NULL;
	bool isClear = true;
	for (int i = 0;i < _width * _height; ++i){
		star = _startArray[i];
		if (NULL != star){
			int currentCol = star->getCol();
			int currentRow = star->getRow();
			//left
			if (currentCol-1 >= 0){
				Block *neighborStar = _startArray[star->getRow() * _width + currentCol - 1];
				if (neighborStar && neighborStar->getIndex() == star->getIndex()){
					isClear = false;
					break;
				}
			}
			//right
			if (currentCol + 1 < _width ){
				Block *neighborStar = _startArray[star->getRow() * _width + currentCol + 1];
				if (neighborStar && neighborStar->getIndex() == star->getIndex()){
					isClear = false;
					break;
				}
			}
			//bottom
			if (currentRow - 1 >= 0){
				Block *neighborStar = _startArray[(currentRow - 1) * _width + currentCol];
				if (neighborStar && neighborStar->getIndex() == star->getIndex()){
					isClear = false;
					break;
				}
			}
			//up
			if (currentRow + 1 < _height){
				Block *neighborStar = _startArray[(currentRow + 1) * _width + currentCol];
				if (neighborStar && neighborStar->getIndex() == star->getIndex()){
					isClear = false;
					break;
				}
			}
		}
	}

	if (isClear){
		_isClearAll = isClear;
		_leaveStar = 0;
		_leaveStarList.clear();
		for (int i = 0; i < _width * _height; i++){
			star = _startArray[i];
			if (star){
				_leaveStarList.push_front(star);
				_leaveStar++;
			}
		}
	}
}

void GameLayer::getStarList(Block* star,std::list<Block* > &starList){
	int currentCol = star->getCol();
	int currentRow = star->getRow();
	//left
	if (currentCol-1 >= 0){
		Block *neighborStar = _startArray[star->getRow() * _width + currentCol - 1];
		if (neighborStar
			&& neighborStar->getIndex() == star->getIndex()
			&& !neighborStar->getIsNeedRemove()){
			neighborStar->setIsNeedRemove(true);
			starList.push_back(neighborStar);
			getStarList(neighborStar,starList);
		}
	}
	//right
	if (currentCol + 1 < _width ){
		Block *neighborStar = _startArray[star->getRow() * _width + currentCol + 1];
		if (neighborStar
			&& neighborStar->getIndex() == star->getIndex()
			&& !neighborStar->getIsNeedRemove()){
			neighborStar->setIsNeedRemove(true);
			starList.push_back(neighborStar);
			getStarList(neighborStar,starList);
		}
	}
	//bottom
	if (currentRow - 1 >= 0){
		Block *neighborStar = _startArray[(currentRow - 1) * _width + currentCol];
		if (neighborStar
			&& neighborStar->getIndex() == star->getIndex()
			&& !neighborStar->getIsNeedRemove()){
			neighborStar->setIsNeedRemove(true);
			starList.push_back(neighborStar);
			getStarList(neighborStar,starList);
		}
	}
	//up
	if (currentRow + 1 < _height){
		Block *neighborStar = _startArray[(currentRow + 1) * _width + currentCol];
		if (neighborStar
			&& neighborStar->getIndex() == star->getIndex()
			&& !neighborStar->getIsNeedRemove()){
			neighborStar->setIsNeedRemove(true);
			starList.push_back(neighborStar);
			getStarList(neighborStar,starList);
		}
	}
}

void GameLayer::getSelectStarList(Block* star){
	_starList.clear();
	_selectList.clear();
	_touchStar->setIsNeedRemove(true);
	_starList.push_back(_touchStar);
	getStarList(_touchStar,_starList);
	//log("size = %d",_starList.size());
	if (_starList.size() >= 2 ){	
		actionForSelectStar();
	}
	else{
		_firstTouch = false;
		_touchStar->setIsNeedRemove(false);
	}
}

void GameLayer::actionForSelectStar(){
	_firstTouch = true;
	for (auto itr = _starList.cbegin(); itr != _starList.cend(); ++itr){

		Sprite *select = Sprite::createWithSpriteFrameName(STAR_SELECT);
		select->setPosition( ((Block*)(*itr))->getPosition() );
		_batchNode->addChild(select);
		_selectList.push_back(select);

		MoveBy* moveUP = MoveBy::create(0.1f,Point(0.0f,4.0f));
		MoveBy* moveDown = MoveBy::create(0.1f,Point(0.0f,-4.0f));
		Sequence *action = Sequence::create(moveUP,moveDown,NULL);

		select->runAction(action);
		((Block*)(*itr))->runAction(action->clone());
	}
	SoundBank::getInstance()->playSelect();
	GameManager::getInstance()->setCanGetScorre(_starList.size() * _starList.size() * 5);
}

void GameLayer::tryRemoveStar(const Point location){
	Rect starRect = Rect(0,0,0,0);
	bool isSelectRemoveStar = false;

	for (auto itr = _starList.cbegin(); itr != _starList.cend(); ++itr){
		Block* star = (Block*)(*itr);
		starRect.origin.x = star->getPositionX() - Block::getContentWidth()/2;
		starRect.origin.y = star->getPositionY() - Block::getContentWidth()/2;
		starRect.size = star->getContentSize();
		if (!starRect.containsPoint(location)) continue;
		else{
			isSelectRemoveStar = true;
			break;
		}
	}

	removeSelectSprite();
	if (isSelectRemoveStar){
		//remove stars
		SoundBank::getInstance()->playSelect();
		if(_starList.size() >= 7){
			showAwesome();
		}
		_firstTouch = false;
		removeStar();
	} 
	else{	
		_firstTouch = false;
		// reset star info
		for (auto itr = _starList.cbegin(); itr != _starList.cend(); ++itr){
			Block* star = (Block*)(*itr);
			star->setIsNeedRemove(false);
		}
		//RePick star
		getSelectStarList(_touchStar);
	}
}

void GameLayer::removeStar(){
	_isAnimationing = true;
	int score = _starList.size() * _starList.size() * 5;
	for (auto itr = _starList.cbegin(); itr != _starList.cend(); ++itr){
		Block* star = (Block*)(*itr);
 		explodeStar(star);
 		_isNeedDrop = true;
	}
	SoundBank::getInstance()->playPopStar();
	GameManager::getInstance()->setCurrentScore(score);
}

void GameLayer::removeSelectSprite(){
	for (auto itr = _selectList.cbegin(); itr != _selectList.cend(); ++itr){
		Sprite* star = (Sprite*)(*itr);
		star->removeFromParent();
	}
}

void GameLayer::explodeStar(Block* star){
	float time = 0.3f;
	star->runAction(Sequence::create(
		ScaleTo::create(time, 0.0f),
		CallFuncN::create(CC_CALLBACK_1(GameLayer::actionEndCallback, this)),
		NULL));
	char fileName[30];
	memset(fileName,0,30);
	sprintf(fileName,PATICLE_NAME.c_str(),star->getIndex());
	auto particleStars = ParticleSystemQuad::create(fileName);
	particleStars->setAutoRemoveOnFinish(true);
	particleStars->setBlendAdditive(false);
	particleStars->setPosition(star->getPosition());
	addChild(particleStars, 20);
}

void GameLayer::actionEndCallback(Node *node)
{
	Block *star = static_cast<Block*>(node);
	_startArray[star->getRow() * _width + star->getCol()] = NULL;
	star->removeFromParent();
}

void GameLayer::drapStar(){
	_isAnimationing = true;
	Size size = CCDirector::getInstance()->getWinSize();
	Block * star = NULL;
	for (int col = 0; col < _width ; ++col){
		int removedStarOfCol = 0;
		for (int row = 0; row < _height; row++){
			star = _startArray[row * _width + col];
			if (NULL == star){
				removedStarOfCol++;
			}else{
				if (removedStarOfCol > 0){
					int newRow = row - removedStarOfCol;
					// switch in matrix
					_startArray[newRow * _width + col] = star;
					_startArray[row * _width + col] = NULL;
					// move to new position
					Point startPosition = star->getPosition();
					Point endPosition = getPointOfBlock(newRow, col);
					float speed = (startPosition.y - endPosition.y) / size.height;
					star->stopAllActions();
					star->runAction(Sequence::create(
						MoveTo::create(speed, endPosition),
						CallFuncN::create(CC_CALLBACK_1(GameLayer::playLandingEffect, this)),
						NULL));
					star->setRow(newRow);				
				}
			}
		}		
	}
	checkLeaveStar();
	//checkMoveLeft();
}

void GameLayer::checkMoveLeft(float dt){
	if (_isAnimationing || _isNeedDrop || _isNeedMoveLeft || _isClearAll){
		return;
	}
	Block *colStar = NULL;
	_nullColCount = 0;
	_startCol = -1;
	int beginnull =  0;

	for (int col = 0; col < _width; col++) {
		colStar = _startArray[col];
		if (NULL == colStar){
			_startCol = col;
			int temp = col;
			beginnull = col;
			int endnull = 0;
			bool allnull = true;
			while(++temp < _width){
				colStar = _startArray[temp];
				if ( NULL != colStar )
				{  
					endnull = temp;
					allnull = false;
					break;
				}
			}
			if (!allnull)
			{
				_nullColCount = endnull - beginnull ;
				break;
			}
			else{
				return;
			}

		}
	}
	if (_startCol > -1 && _nullColCount >= 1 )
	{
		_touchEnabled = false;
		_isNeedMoveLeft = true;
	}else{
		_isNeedMoveLeft = false;
	}
}

void GameLayer::move2Left(int startCol,int nullCount){
	if (startCol == _width){
		return;
	}
	_touchEnabled = false;	
	_isAnimationing = true;
	Block *star = NULL;
	for (int col = startCol+1; col < _width ; ++col){
		for (int row = 0; row < _height; row++){
			star = _startArray[row * _width + col];
			if (NULL != star ){				
				int newCol = col - nullCount;
				//log("row = %d, col = %d, newcol = %d",row,col,newCol);
				// switch in matrix
				_startArray[row * _width + newCol] = star;
				_startArray[row * _width + col] = NULL;
				Point endPosition = getPointOfBlock(row, newCol);
				star->stopAllActions();
				star->runAction(MoveTo::create(0.15f, endPosition));
				star->setCol(newCol);
			}
		}
	}
}

void GameLayer::playLandingEffect(Node *node){
	SoundBank::getInstance()->playLanding();
}

void GameLayer::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event){
	if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE){
		saveStarData();
		GameManager::getInstance()->backToMain();
	}
}

void GameLayer::saveStarData(){
	char key[30];
	memset(key,0,30);
	Block *star = nullptr;
	if (!_isGameOver){
		USEDATA->setIntegerForKey(CURRENTSCORE,GameManager::getInstance()->getCurrentScore());
		USEDATA->setBoolForKey(CONTINUEGAME,true);
		USEDATA->setIntegerForKey(CURRENTLEVEL,GameManager::getInstance()->getCurrentLevel());

		for (int row = 0; row < _height; ++row){//行
			for(int col = 0; col < _width ;++col){//列
				star = _startArray[row * _width + col];
				sprintf(key,"row_%dcol_%d",row,col);
				if (star){
					USEDATA->setIntegerForKey(key,star->getIndex());
				}else{
					USEDATA->setIntegerForKey(key,-1);
				}
			}
		}
	}
	else{
		USEDATA->setBoolForKey(CONTINUEGAME,false);
		USEDATA->setIntegerForKey(CURRENTSCORE,0);
		USEDATA->setIntegerForKey(CURRENTLEVEL,0);
		for (int row = 0; row < _height; ++row){//行
			for(int col = 0; col < _width ;++col){//列
				star = _startArray[row * _width + col];
				sprintf(key,"row_%dcol_%d",row,col);
				USEDATA->setIntegerForKey(key,-1);
			}
		}
	}

	UserDefault::getInstance()->flush();  
}

void GameLayer::showAwesome(){
	SoundBank::getInstance()->playerAwesome();
	Size winsize = Director::getInstance()->getWinSize();
	auto sprite = Sprite::create(COMBO_AWESOME);
	sprite->setPosition(Point(winsize.width/2,winsize.height/2));
	this->addChild(sprite,1);

	sprite->runAction(
		Sequence::create(
		Blink::create(1.5f,3),
		Spawn::create(MoveBy::create(0.5f,Point(0.0f,winsize.height/4)),
						ScaleTo::create(0.5f,0.5f),NULL),
		CallFuncN::create(CC_CALLBACK_1(GameLayer::removeAwesome, this)),
		NULL));
}

void GameLayer::removeAwesome(Node *node){
	Sprite* sprite = (Sprite*)node;
	sprite->removeFromParent();
}

void GameLayer::gameOver(){
	_touchEnabled = false;
	
	Size winsize = Director::getInstance()->getWinSize();
	SoundBank::getInstance()->playGameOver();
	TTFConfig config("fonts/tahoma.ttf",25);
	auto label =  Label::createWithTTF(config,"Game Over !",TextHAlignment::LEFT);
	label->setPosition(Point(winsize.width + 100.0f,winsize.height/2));
	this->addChild(label);

	label->runAction(Sequence::create(
		MoveTo::create(0.5f,Point(winsize.width/2,winsize.height/2)),
		Blink::create(1.5f,3),
		DelayTime::create(2.0f),
		CallFuncN::create(CC_CALLBACK_1(GameLayer::goMainScene, this)),
		NULL));
}

void GameLayer::goMainScene(Node *node){
	node->removeFromParent();
	SoundBank::getInstance()->stopPlayEffect();
	if (Rank::getinstance()->checkCanSave(GameManager::getInstance()->getCurrentScore()))
	{
		GameManager::getInstance()->displaySaveLayer();
	}
	else{
		GameManager::getInstance()->backToMain();
	}
	
}

void GameLayer::checkLeaveStar(){
	if (_isGameOver)
	{
		return;
	}
	int count = 0;
	for (int i = 0; i < _height * _width; i++) {
		Block *star = _startArray[i];
		if (star) {
			count++;
		}
	}
	GameManager::getInstance()->setLeaveStar(count);
}