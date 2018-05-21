#ifndef _KUNAI_H_
#define _KUNAI_H_
#include "Model\Weapon.h"

class Kunai: public Weapon
{
public:
	Kunai(cocos2d::Layer* layer);
	~Kunai();
};

#endif