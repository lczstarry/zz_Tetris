#include "EditBoxManager.h"  
//#include "VisibleRect.h"  
#include "HelloWorldScene.h"
#define FONT_NAME "fonts/Marker Felt.ttf"  
#define FONT_SIZE 26  

void runEditBoxManager() {
	HelloWorld* pCurrentScene = (HelloWorld*)CCDirector::getInstance()->getRunningScene();
	EditBoxManager* layer = new EditBoxManager;
	layer->autorelease();  //交给内存池进行管理  
	pCurrentScene->addChild(layer);
	
	//Director::getInstance()->runWithScene(pCurrentScene);
}

EditBoxManager::EditBoxManager() {

	m_size = CCDirector::sharedDirector()->getVisibleSize();
	playerName = "";

	auto s = Size(250, 50);  //设置编辑框大小  
							 //Scale9Sprite类似android上的9图工具,可对图片进行拉伸而不失真  
	auto m9pic = Scale9Sprite::create("bg.png");

	mEditBox = EditBox::create(s, m9pic);
	mEditBox->setFontName(FONT_NAME);        //编辑框文本字体  
	mEditBox->setFontSize(FONT_SIZE);        //编辑框文本大小  
	mEditBox->setFontColor(Color3B::BLUE);  //编辑框文本颜色  
	mEditBox->setPlaceHolder("YOUR NAME");  //编辑框提示语句  
	mEditBox->setPlaceholderFontColor(Color3B::GRAY); //编辑框提示语句颜色  
	mEditBox->setMaxLength(10);             //编辑框文本长度  
	mEditBox->setInputFlag(EditBox::InputFlag::INITIAL_CAPS_ALL_CHARACTERS);//编辑框文本框输入类型  
	mEditBox->setInputMode(EditBox::InputMode::SINGLE_LINE); //编辑框文本的输入模式  
	mEditBox->setDelegate(this);            //注册编辑框协议  
	//mEditBox->setPosition(VisibleRect::center());
	mEditBox->setReturnType(EditBox::KeyboardReturnType::GO);
	mEditBox->setPosition(ccp(m_size.width / 2, m_size.height / 2+20));
	addChild(mEditBox);
}

void EditBoxManager::editBoxEditingDidBegin(EditBox* editBox) {
	log("click");
}
void EditBoxManager::editBoxEditingDidEnd(EditBox* editBox) {
	log("leave click");
}
void EditBoxManager::editBoxTextChanged(EditBox* editBox, const std::string& text) {
	log("textChanged!");
	CCLabelTTF * ttf = CCLabelTTF::create(text, "", 24);
	ttf->setPosition(ccp(m_size.width / 4+100, m_size.height * 3 / 5));
	this->addChild(ttf);
	if(text.length()==3)
		this->playerName = text;
}
void EditBoxManager::editBoxReturn(EditBox* editBox) {


	/*log("return");
	CCLabelTTF * ttf = CCLabelTTF::create("return", "", 24);
	ttf->setPosition(ccp(m_size.width / 2, m_size.height / 2+50 ));
	this->addChild(ttf);

	char * str = (char *)this->mEditBox->getText();
	CCLabelTTF * text = CCLabelTTF::create(str, "", 24);
	text->setPosition(ccp(CCDirector::sharedDirector()->getVisibleSize().width / 2, CCDirector::sharedDirector()->getVisibleSize().height / 2));
	this->addChild(text);*/

}

void  EditBoxManager::getBoxText()
{
	string str = this->playerName;
}