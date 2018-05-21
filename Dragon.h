#ifndef _DRAGON_H_
#define _DRAGON_H_
#include "Model\Model.h"
#include "Model\BulletDragon.h"

#include "cocos2d.h"
#include "Define.h"

class Dragon : public Model
{
private:
	int check2 = 0;
	int actionCurren;
	BulletDragon* bulletDragon;

public:

	bool Shooting(float xTarget, float yTarget);

	Dragon(cocos2d::Layer* layer);

	~Dragon();

	cocos2d::Action* actionOfDragon[1];

	void update();

	void hasCollision();

	void setIndex(int index);
	void fly();
};

#endif