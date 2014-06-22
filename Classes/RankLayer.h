#include "UseDefine.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"

USING_NS_CC_EXT;
using namespace ui;

class RankLayer :public Layer
{
public:
	RankLayer();
	~RankLayer();
	virtual bool init() override;
	void selectedItemEvent(Ref* pSender, ListView::EventType type);
	CREATE_FUNC(RankLayer);
	void setRank();
	virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
private:
	ListView* listView ;
};

