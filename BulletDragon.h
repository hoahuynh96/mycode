#ifndef _BULLETDRAGON_H_
#define _BULLETDRAGON_H_

#include "Model\Weapon.h"
#include "cocos2d.h"

class BulletDragon : public Weapon
{


public:

	BulletDragon(cocos2d::Layer* layer);
	~BulletDragon();
	void setRowtation(float angle);
};


#endif