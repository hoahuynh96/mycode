#ifndef _CREP_H_
#define _CREP_H_
#include "Model\Model.h"
#include "Model\BulletDragon.h"

#include "cocos2d.h"
#include "Define.h"

class Crep : public Model
{
private:
	int actionCurren;
	BulletDragon* bulletDragon;

public:



	Crep(cocos2d::Layer* layer);

	~Crep();

	cocos2d::Action* actionOfCrep[2];

	//void update();

	void hasCollision();

	void setIndex(int index);
	void go();
	void actack();
	void runCrep(float x, float y);
	void dame();
	
};

#endif