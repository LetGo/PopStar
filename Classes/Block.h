#pragma once
#include "cocos2d.h"

class Block :public cocos2d::Sprite
{
public:
	Block();
	~Block();
	static Block* create(int row,int col);
	static Block* createByIndex(int row,int col,unsigned int index);
	CC_SYNTHESIZE(int, _row,Row);
	CC_SYNTHESIZE(int, _col,Col);
	CC_SYNTHESIZE(int, _index,Index);
	CC_SYNTHESIZE(bool,_isNeedRemove,IsNeedRemove);
	static float getContentWidth();
	std::string getColor(int index);
	void toString();
};

