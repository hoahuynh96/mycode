#ifndef _POWER_SWORD_
#define _POWER_SWORD_
#include "Model\Weapon.h"
#include "cocos2d.h"

class PowerSword: public Weapon
{
public:
	PowerSword(cocos2d::Layer* layer);
	~PowerSword();
};

#endif