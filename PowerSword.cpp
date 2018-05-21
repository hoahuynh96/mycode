#include "Model\powerSword.h"
#include "Define.h"

USING_NS_CC;

PowerSword::PowerSword(Layer* layer)
{
	mSprite = Sprite::createWithSpriteFrameName("kiemkhi.png");
	mSprite->setPosition(Vec2(-100.0f, -100.0f));
	layer->addChild(mSprite, 10);

	mPhysicBody = PhysicsBody::createBox(mSprite->getContentSize(), PhysicsMaterial(1.0f, 0.0f, 1.0f));
	mPhysicBody->setCollisionBitmask(POWER_BISMARK);
	mPhysicBody->setContactTestBitmask(true);
	mPhysicBody->setGravityEnable(false);
	mPhysicBody->setMass(300.0f);

	mSprite->setPhysicsBody(mPhysicBody);

	this->setIsDislay(false);
}

PowerSword::~PowerSword()
{

}