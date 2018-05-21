#ifndef _INTRO_H_
#define _INTRO_H_

#include "cocos2d.h"
#include "ui\CocosGUI.h"
USING_NS_CC;

class Intro : public Layer {
private:
	Sprite* intro;
	bool isIntroFinish;
	Action* mAnimation[1];
	ui::Button* skipButton;
public:
	static Scene* createScene();
	virtual bool init();
	virtual void update(float dt);
	void setupSkipButton();
	CREATE_FUNC(Intro);
};
#endif
