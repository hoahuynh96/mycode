#ifndef _WAIT_SCENE_H_
#define _WAIT_SCENE_H_
#include "cocos2d.h"

class WaitScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	CREATE_FUNC(WaitScene);
};

#endif