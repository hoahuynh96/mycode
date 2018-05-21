#ifndef _LOADSCENE_H_
#define _LOADSCENE_H_
#include "cocos2d.h"
#include "ui\CocosGUI.h"

class LoadScene : public cocos2d::Layer
{
private:
	cocos2d::Sprite* loadingBG;
	cocos2d::ui::LoadingBar* loadingBar;

public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	virtual void update(float dt);

	CREATE_FUNC(LoadScene);
};
#endif