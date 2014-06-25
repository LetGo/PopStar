#include "UseDefine.h"

class info{
public:
	std::string name;
	int score;
};

class Rank
{
public:
	Rank();
	~Rank();
	static Rank* getinstance();
	void init();
	void save(const char* name,int score);
	std::list<info*> getList();
	bool checkCanSave(const int score);
	void unInit();
private:
	std::string path;
	static Rank* _instance;
	Dictionary* _rankDict;
	
};

