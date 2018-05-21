#ifndef _BULLET_FIRE_H_
#define _BULLET_FIRE_H_

#include "Model\Weapon.h"
#include "cocos2d.h"

class BulletFire : public Weapon
{
public:
	BulletFire(cocos2d::TMXTiledMap* layer);
	~BulletFire();
};

#endif