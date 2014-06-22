#include "SoundBank.h"

SoundBank* SoundBank::_instance = nullptr;

SoundBank::SoundBank():
	_enable(false)
{
}

SoundBank::~SoundBank()
{
	log("SoundBank -----");
}

SoundBank* SoundBank::getInstance(){
	if (_instance == nullptr){
		_instance = new SoundBank();
	}
	return _instance;
}

void SoundBank::init(){
	if(!USEDATA->getBoolForKey(SOUND_ENABEL)){
		USEDATA->setBoolForKey(SOUND_ENABEL,false);

		USEDATA->flush();
	}
	USEDATA->setBoolForKey(SOUND_ENABEL,false);
	_enable = USEDATA->getBoolForKey(SOUND_ENABEL);

	AudioEngine->preloadEffect("Sounds/pop_star.wav");
	AudioEngine->preloadEffect("Sounds/button_start.wav");
	AudioEngine->preloadEffect("Sounds/applause.wav");
	AudioEngine->preloadEffect("Sounds/gameover.wav");
	AudioEngine->preloadEffect("Sounds/clear.wav");
	AudioEngine->preloadEffect("Sounds/cheers.wav");
	AudioEngine->preloadEffect("Sounds/select.wav");
	AudioEngine->preloadEffect("Sounds/landing.wav");
	AudioEngine->preloadEffect("Sounds/logo.wav");
}

void SoundBank::unInit(){
	AudioEngine->unloadEffect("Sounds/pop_star.wav");
	AudioEngine->unloadEffect("Sounds/button_start.wav");
	AudioEngine->unloadEffect("Sounds/applause.wav");
	AudioEngine->unloadEffect("Sounds/gameover.wav");
	AudioEngine->unloadEffect("Sounds/clear.wav");
	AudioEngine->unloadEffect("Sounds/cheers.wav");
	AudioEngine->unloadEffect("Sounds/select.wav");
	AudioEngine->unloadEffect("Sounds/landing.wav");
	AudioEngine->unloadEffect("Sounds/logo.wav");
	if (_instance){
		delete _instance;
	}
}

void SoundBank::playBackgound(){
	//AudioEngine->playBackgroundMusic("Sounds/daybyday.mp3", true);
}

void SoundBank::playPopStar(){
	if (!getEnable()){
		return;
	}
	AudioEngine->playEffect("Sounds/pop_star.wav", false);
}

void SoundBank::playMenuStart(){
	if (!getEnable()){
		return;
	}
	AudioEngine->playEffect("Sounds/button_start.wav", false);
}

void SoundBank::playClear(){
	if (!getEnable()){
		return;
	}
	AudioEngine->playEffect("Sounds/clear.wav", false);
}

void SoundBank::playerAwesome(){
	if (!getEnable()){
		return;
	}
	AudioEngine->playEffect("Sounds/applause.wav");
}

void SoundBank::playGameOver(){
	if (!getEnable()){
		return;
	}
	AudioEngine->playEffect("Sounds/gameover.wav");
}

void SoundBank::playCheers(){
	if (!getEnable()){
		return;
	}
	AudioEngine->playEffect("Sounds/cheers.wav");
}

void SoundBank::stopPlayEffect(){
	AudioEngine->stopAllEffects();
}

void SoundBank::playSelect(){
	if (!getEnable()){
		return;
	}
	AudioEngine->playEffect("Sounds/select.wav");
}

void SoundBank::playLanding(){
	if (!getEnable()){
		return;
	}
	AudioEngine->playEffect("Sounds/landing.wav");
}

void SoundBank::playLogo(){
	if (!getEnable()){
		return;
	}
	AudioEngine->playEffect("Sounds/logo.wav");
}

Player::Player()
{
}

Player::~Player()
{
}

void Player::add(std::string path){

}