#include "cocos2d.h"
using namespace cocos2d;

class TextBank
{
public:
	TextBank();
	~TextBank();
	static TextBank* getInstance();
	void init();
	void unInit();
	std::string getLevelText() ;
	std::string getTargetText() ;
	std::string getGameOverText() ;
	std::string getCanGetScoreText();
	std::string getGameName();
	std::string getTypeName();
private:
	static TextBank* _instance;
	std::string _level;
	std::string _target;
	std::string _gameOver;
	std::string _canGetScore;
	std::string _name;
	std::string _typeName;
};

