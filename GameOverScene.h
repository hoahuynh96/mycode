#ifndef _GAME_OVER_SCENE_H_
#define _GAME_OVER_SCENE_H_
#include "cocos2d.h"

class GameOverScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();
	
	CREATE_FUNC(GameOverScene);
};

#endif