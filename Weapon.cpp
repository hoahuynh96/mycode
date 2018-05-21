#include "Model\Weapon.h"
#include "Define.h"

USING_NS_CC;

Weapon::Weapon()
{

}

Weapon::~Weapon()
{

}

void Weapon::update()
{
	float x = mSprite->getPositionX();
	float y = mSprite->getPositionY();
	mSprite->setPosition(x - moveByX, y - moveByY);

	/*if (x > SCENE_W || y > SCENE_H || x < 0 || y < 0)
	{
		this->setIsDislay(false);
	}*/

	if (mSprite->getParent()->getPositionX() + x > SCENE_W || mSprite->getParent()->getPositionY() + y > SCENE_H || mSprite->getParent()->getPositionX() + x < 0 || mSprite->getParent()->getPositionY() + y < 0)
	{
		this->setIsDislay(false);
	}
	
	
}

void Weapon::setMoveByX(float x)
{
	moveByX = x;
}

void Weapon::setMoveByY(float y)
{
	moveByY = y;
}

void Weapon::setIsDislay(bool isDislay)
{
	mSprite->setVisible(isDislay);
	this->isDislay = isDislay;
	mPhysicBody->setEnabled(isDislay);
}

bool Weapon::getIsDislay()
{
	return isDislay;
}

void Weapon::setPosition(float x, float y)
{
	mSprite->setPosition(Vec2(x,y));
}

void Weapon::setFlippedX(bool flipped)
{
	mSprite->setFlippedX(flipped);
}

Size Weapon::getContentSize()
{
	return mSprite->getContentSize();
}

void Weapon::setIndexGroup(int index)
{
	mPhysicBody->setGroup(index);
}