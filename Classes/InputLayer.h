#include "UseDefine.h"
#include "extensions/cocos-ext.h"
USING_NS_CC_EXT;

class InputLayer : public Layer,public cocos2d::extension::EditBoxDelegate
{
public:
	InputLayer();
	~InputLayer();
	virtual bool init();
	CREATE_FUNC(InputLayer);
	virtual void editBoxEditingDidBegin(cocos2d::extension::EditBox* editBox);
	virtual void editBoxEditingDidEnd(cocos2d::extension::EditBox* editBox);
	virtual void editBoxTextChanged(cocos2d::extension::EditBox* editBox, const std::string& text);
	virtual void editBoxReturn(cocos2d::extension::EditBox* editBox);
	void onConfirm(Ref* pSender);
private:
	int                 _charLimit;    
	cocos2d::extension::EditBox* _editName;
};

