#include "EditBoxManager.h"  
//#include "VisibleRect.h"  
#include "HelloWorldScene.h"
#define FONT_NAME "fonts/Marker Felt.ttf"  
#define FONT_SIZE 26  

void runEditBoxManager() {
	HelloWorld* pCurrentScene = (HelloWorld*)CCDirector::getInstance()->getRunningScene();
	EditBoxManager* layer = new EditBoxManager;
	layer->autorelease();  //�����ڴ�ؽ��й���  
	pCurrentScene->addChild(layer);
	
	//Director::getInstance()->runWithScene(pCurrentScene);
}

EditBoxManager::EditBoxManager() {

	m_size = CCDirector::sharedDirector()->getVisibleSize();
	playerName = "";

	auto s = Size(250, 50);  //���ñ༭���С  
							 //Scale9Sprite����android�ϵ�9ͼ����,�ɶ�ͼƬ�����������ʧ��  
	auto m9pic = Scale9Sprite::create("bg.png");

	mEditBox = EditBox::create(s, m9pic);
	mEditBox->setFontName(FONT_NAME);        //�༭���ı�����  
	mEditBox->setFontSize(FONT_SIZE);        //�༭���ı���С  
	mEditBox->setFontColor(Color3B::BLUE);  //�༭���ı���ɫ  
	mEditBox->setPlaceHolder("YOUR NAME");  //�༭����ʾ���  
	mEditBox->setPlaceholderFontColor(Color3B::GRAY); //�༭����ʾ�����ɫ  
	mEditBox->setMaxLength(10);             //�༭���ı�����  
	mEditBox->setInputFlag(EditBox::InputFlag::INITIAL_CAPS_ALL_CHARACTERS);//�༭���ı�����������  
	mEditBox->setInputMode(EditBox::InputMode::SINGLE_LINE); //�༭���ı�������ģʽ  
	mEditBox->setDelegate(this);            //ע��༭��Э��  
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