#include "Model\BulletDragon.h"

#include "Define.h"

USING_NS_CC;

BulletDragon::BulletDragon(cocos2d::Layer* layer)
{

	mSprite = Sprite::create("bullets_01.png");
	mSprite->setScale(0.3f);

	mPhysicBody = PhysicsBody::createBox(mSprite->getContentSize(), PhysicsMaterial(1.0f, 0.0f, 1.0f));
	mPhysicBody->setGravityEnable(false);
	mPhysicBody->setRotationEnable(false);
	mPhysicBody->setCollisionBitmask(BULLETDRAGON_BISMARK);
	mPhysicBody->setGravityEnable(false);
	mPhysicBody->setMass(100);
	mPhysicBody->setContactTestBitmask(true);

	mSprite->setPhysicsBody(mPhysicBody);

	layer->addChild(mSprite, 1);
	this->setIsDislay(false);
}
BulletDragon::~BulletDragon()
{

}

void BulletDragon::setRowtation(float angle)
{
	mSprite->setRotation(angle);
}