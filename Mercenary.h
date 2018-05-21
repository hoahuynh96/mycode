#ifndef _MERCENARY_H_
#define _MERCENARY_H_
#include "Model\Model.h"
#include "Model\BulletFire.h"

#include "cocos2d.h"

class Mercenary: public Model
{
public:
	Mercenary(cocos2d::TMXTiledMap* layer);

	~Mercenary();

	int getDamage();

	void Run();

	void Attack();

	void Fire();

	void setIndex(int index);

	void setMinPosition(int minPosition);

	int getMinPosition();

	void setMaxPosition(int maxPosition);

	int getMaxPosition();

	void Update();

	void autoWalk();

	void chase(float x);

	void hasCollistion();

	void updateFire();
private:
	cocos2d::Action* actionOfMercenary[3];

	int currentAction;

	BulletFire* bulletFire;

	int minPosition;

	int maxPosition;

	void Damage();
};

#endif