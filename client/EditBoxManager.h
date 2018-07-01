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

	//���༭���ý���ʱ��������  
	virtual void editBoxEditingDidBegin(EditBox* editBox);
	//���༭��ʧȥ����󽫱�����  
	virtual void editBoxEditingDidEnd(EditBox* editBox);
	//���༭�����ݷ����ı佫������  
	virtual void editBoxTextChanged(EditBox* editBox, const std::string& text);
	//���༭��Ľ�������������  
	virtual void editBoxReturn(EditBox* editBox);


	void getBoxText();
	EditBox * mEditBox;

	string  playerName;
	CCSize m_size;

};

void runEditBoxManager();

#endif  //EDIT_BOX_MANAGER  