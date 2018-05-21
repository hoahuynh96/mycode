#ifndef _INFORACTORSCENE_H_
#define _INFORACTORSCENE_H_

#include "cocos2d.h"

class InforActorScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();
	CREATE_FUNC(InforActorScene);
};
#endif