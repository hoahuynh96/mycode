#ifndef _WEAPON_H_
#define _WEAPON_H_
#include "cocos2d.h"

class Weapon
{
protected:
	int damage;

	float moveByX;

	float moveByY;

	bool isDislay;

	cocos2d::Sprite* mSprite;

	cocos2d::PhysicsBody* mPhysicBody;
public:
	Weapon();

	~Weapon();

	void update();

	void setMoveByX(float x);

	void setMoveByY(float y);

	void setIsDislay(bool isDislay);

	bool getIsDislay();

	void setPosition(float x,float y);

	void setFlippedX(bool flipped);

	cocos2d::Size getContentSize();

	void setIndexGroup(int index);
};

#endif