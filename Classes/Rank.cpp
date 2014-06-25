#include "Rank.h"

Rank* Rank::_instance = nullptr;
const std::string fileName =  "rank.plist";

const int MAXLIST = 10;

Rank::Rank()
{
}

Rank::~Rank()
{
	
}

 Rank* Rank::getinstance(){
	 if (!_instance){
		 _instance = new Rank();
	 }
	 return _instance;
 }

 void Rank::init(){
	 
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	 path = FileUtilsAndroid::getInstance()->getWritablePath() + fileName;
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	 path = FileUtils::getInstance()->getWritablePath() + fileName;
#endif
	 //log("=====================path======================:%s",path.c_str());
	_rankDict = Dictionary::createWithContentsOfFile(path.c_str());
	_rankDict->retain();
	_rankDict->writeToFile(path.c_str());	
 }

 void Rank::unInit(){
	 _rankDict->release();
	 if (_instance){
		 delete _instance;
	 }
 }

 void Rank::save(const char* name,int score){
	 char key[20] = {0};
	 sprintf(key,"%d",score);
	 _rankDict->setObject(CCString::create(name),key);
	 _rankDict->writeToFile(path.c_str());
 }

 std::list<info*> Rank::getList(){
	 std::list<info*> minfo;
	 std::list<int> _scoreList;
	 char obj[20] = {0};
	 auto arrays =  _rankDict->allKeys();
	 if (!arrays){
		 return minfo;
	 }
	 for (int i = 0; i < arrays->count(); ++i){
		int socre = ((CCString*)arrays->getObjectAtIndex(i))->intValue();
		_scoreList.push_back(socre);		 
	 }	
	 _scoreList.sort();
	 _scoreList.reverse();

	 for (auto itr = _scoreList.cbegin(); itr != _scoreList.cend(); itr++){
		 sprintf(obj,"%d",*itr);
		 auto object = _rankDict->objectForKey(obj);
		 if (object){
			 auto temp = new info();
			 temp->score = *itr;
			 temp->name = ((CCString*)object)->_string;
			 minfo.push_back(temp);
		 }	 
	 }
	 return minfo;
 }

 bool Rank::checkCanSave(const int score){
	 auto sarray = _rankDict->allKeys();
	 if (!sarray || sarray->count() <10){
		 return true;
	 }
	 for (int i = 0; i < sarray->count(); i++){
		 int s  = ((CCString*)sarray->objectAtIndex(i))->intValue();
		 if (score >= s){
			 _rankDict->removeObjectForKey(((CCString*)sarray->objectAtIndex(i))->_string);
			 return true;
		 }
	 }
	 return false;
 }