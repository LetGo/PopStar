#include "UseDefine.h"
#include "SimpleAudioEngine.h"
#include<iostream>
#include<queue>
using namespace std;
using namespace CocosDenshion;

class SoundBank
{
public:
	SoundBank();
	~SoundBank();
	static SoundBank* getInstance();
	void init();
	void unInit();
	CC_SYNTHESIZE(bool,_enable,Enable);
	void playBackgound();
	void playPopStar();
	void playMenuStart();
	void playClear();
	void playerAwesome();
	void playGameOver();
	void stopPlayEffect();
	void playCheers();
	void playSelect();
	void playLanding();
	void playLogo();
private:
	static SoundBank* _instance ;
};

class Player :public Node
{
public:
	Player();
	~Player();
	void add(std::string path);
private:
	queue<string> soundQueue; //ÉùÃ÷¶ÓÁÐ
};


