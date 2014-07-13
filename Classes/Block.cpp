#include <iostream>
#include <unordered_map>
#include <random>
#include <time.h>
#include "Block.h"
#include "UseDefine.h"

// #ifndef M_DEBUG
// #define M_DEBUG
// #endif // !M_DEBUG

const int TORAL_BLOCK = 5;
std::default_random_engine generator(time(0));
std::uniform_int_distribution<int> distribution(0,TORAL_BLOCK-1);

static const char *block[TORAL_BLOCK] = {
	"block_blue.png",
	"block_green.png",
	"block_red.png",
	"block_yellow.png",
	"block_purple.png"
};

Block::Block():
	_col(0),
	_index(0),
	_isNeedRemove(false),
	_row(0)
{
}

Block::~Block()
{
}

Block* Block::create(int row,int col){
	return createByIndex( row,col,distribution(generator) );
}

Block* Block::createByIndex(int row,int col,unsigned int index){
	
	CCASSERT(row >= 0 && row < 10 && col >= 0 && col < 10 && index >= 0 && index < 5, "create block faild");

	if (row < 0 || row >= 10 || col < 0 || col >= 10 || index < 0 || index >= 5 )
	{
		log("create block faild row =%d,col = %d index = %d",row,col,index);		
		return NULL;
	}
	Block* sprite = new Block();
	int m_index = index;
#ifdef M_DEBUG
	char str[15];
	memset((void*)str,0,15);
	sprintf(str,"debug_%d_%d",row,col);
	if (USEDATA->getIntegerForKey(str))
	{
		m_index = USEDATA->getIntegerForKey(str);
	}
#endif // M_DEBUG
	sprite->_row = row;
	sprite->_col = col;
	sprite->_index = m_index;
	sprite->initWithSpriteFrameName(block[m_index]);
	sprite->autorelease();
	return sprite;
}

float Block::getContentWidth()
{
	static float itemWidth = 0;
	if (0 == itemWidth) {
		Sprite *sprite = CCSprite::createWithSpriteFrameName(block[0]);
		itemWidth = sprite->getContentSize().width;
	}
	return itemWidth;
}

void Block::toString(){
	log("{\n\tROW = %d,\n \tCOL = %d,\n\tindex = %d,\n\tcolor = %s\n}",this->getRow(),this->getCol(),this->getIndex(),this->getColor(this->getIndex()).c_str());
}

std::string Block::getColor(int index){
	std::string color;
	if (index == 0)
	{
		color = "blue";
	} 
	else if (index == 1)
	{
		color = "green";
	}
	else if (index == 2)
	{
		color = "red";
	}
	else if (index == 3)
	{
		color = "yellow";
	}
	else if (index == 4)
	{
		color = "purple";
	}

	return color;
}

//EOF