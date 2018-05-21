#ifndef _WIN_SCENE_H_
#define _WIN_SCENE_H_
#include "Model\GameSetting.h"
#include "cocos2d.h"

class WinScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	GameSetting* gameSetting;
	virtual bool init();

	CREATE_FUNC(WinScene);
};

#endif