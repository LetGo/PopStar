#include "AppDelegate.h"
#include "HelloWorldScene.h"
#include "GameManager.h"
#include "TextBank.h"
USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
	GameManager::getInstance()->UnInit();
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
	GameManager::getInstance()->init();
    if(!glview) {
        glview = GLView::create(TextBank::getInstance()->getGameName());
        director->setOpenGLView(glview);
    }
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
		glview->setFrameSize(320,480);
#endif
	glview->setDesignResolutionSize(320.0f,480.0f,ResolutionPolicy::FIXED_WIDTH);
	//std::vector<std::string> searchPath;
	//searchPath.push_back("640");
	//searchPath.push_back("block");
	//CCFileUtils::getInstance()->setSearchPaths(searchPath);
	director->setContentScaleFactor(640.0f/320.0f);
    // turn on display FPS

    director->setDisplayStats(GameManager::getInstance()->getDebug());

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
  //  auto scene = HelloWorld::createScene();

    // run
    director->runWithScene(GameManager::getInstance()->startScene());
	//GameManager::getInstance()->runStartScene();
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
