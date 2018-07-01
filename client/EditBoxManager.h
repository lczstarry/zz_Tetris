#ifndef EDIT_BOX_MANAGER  
#define EDIT_BOX_MANAGER  
#include "cocos2d.h"  
#include "extensions/cocos-ext.h"  
#include <string>

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::extension;
using namespace cocos2d;
using namespace std;
class EditBoxManager :public Layer, public EditBoxDelegate {
public:
	EditBoxManager();

	//当编辑框获得焦点时将被调用  
	virtual void editBoxEditingDidBegin(EditBox* editBox);
	//当编辑框失去焦点后将被调用  
	virtual void editBoxEditingDidEnd(EditBox* editBox);
	//当编辑框内容发生改变将被调用  
	virtual void editBoxTextChanged(EditBox* editBox, const std::string& text);
	//当编辑框的结束操作被调用  
	virtual void editBoxReturn(EditBox* editBox);


	void getBoxText();
	EditBox * mEditBox;

	string  playerName;
	CCSize m_size;

};

void runEditBoxManager();

#endif  //EDIT_BOX_MANAGER  