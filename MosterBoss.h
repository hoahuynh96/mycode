#ifndef _MOSTERBOSS_H_
#define _MOSTERBOSS_H_
#include"Define.h"
#include "Model.h"
#include "cocos2d.h"
class MosterBoss : public Model
{
public:
	int i = 0;

	
	MosterBoss(cocos2d::Layer* layer);
	~MosterBoss();


	void runBoss(float x, float y);
	void actack();

	void skill();
	void runback();
	void kich(float x);
	void dame();
	void blood_loss(float);
	void enable();
private :
	int actionCurren;
	cocos2d::Action* lisAction[4];

	
};

#endif