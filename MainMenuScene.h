#ifndef _MAINMENUSCENE_H_
#define _MAINMENUSCENE_H_
#include "cocos2d.h"

class MainMenuScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();


	CREATE_FUNC(MainMenuScene);
};
#endif