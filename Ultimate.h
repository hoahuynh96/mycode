#ifndef _ULTIMATE_H_
#define _ULTIMATE_H_
#include "Model\Model.h"
#include "cocos2d.h"

class Ultimatel : public Model
{
private:
	float speedfall;
	int actionCurren;
	cocos2d::Action* lisAction[1];
	
public:
	void fall();
	Ultimatel(cocos2d::Layer* layer);
	~Ultimatel();
};

#endif