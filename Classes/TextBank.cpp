#include "TextBank.h"

TextBank* TextBank::_instance = nullptr;

TextBank::TextBank()
{
}

TextBank::~TextBank()
{
	log("TextBank -----");
}

TextBank* TextBank::getInstance(){
	if (_instance == nullptr){
		_instance = new TextBank();	
	}
	
	return _instance;
}

void TextBank::init(){
	auto fontDic = Dictionary::createWithContentsOfFile("string.xml");
	_level = ((CCString*)fontDic->objectForKey("level"))->_string;
	_target = ((CCString*)fontDic->objectForKey("target"))->_string;
	_gameOver = ((CCString*)fontDic->objectForKey("gameOver"))->_string;
	_canGetScore = ((CCString*)fontDic->objectForKey("canGetScore"))->_string;
	_name = ((CCString*)fontDic->objectForKey("gameName"))->_string;
	_typeName = ((CCString*)fontDic->objectForKey("typeName"))->_string;
	_git = ((CCString*)fontDic->objectForKey("git"))->_string;
}

void TextBank::unInit(){
	_level = "";
	_target = "";
	_gameOver = "";
	_canGetScore = "";
	_name = "";
	_typeName = "";
	_git = "";
	if (_instance){
		delete _instance;
	}
}

std::string TextBank::getLevelText() {
	return _level;
}

std::string TextBank::getTargetText() {

	return _target;
}

std::string TextBank::getGameOverText() {
	return _gameOver;
}

std::string TextBank::getCanGetScoreText(){
	return _canGetScore;
}

std::string TextBank::getGameName(){
	return _name;
}

std::string TextBank::getTypeName(){
	return _typeName;
}

std::string TextBank::getGitAdd(){
	return _git;
}