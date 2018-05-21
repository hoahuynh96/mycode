#include "Model\BulletFire.h"

#include "Define.h"

USING_NS_CC;

BulletFire::BulletFire(TMXTiledMap* layer)
{
	damage = 20;

	mSprite = Sprite::createWithSpriteFrameName("Mercenary_fire.png");
	mSprite->setPosition(Vec2(-200.0f, -200.0f));
	layer->addChild(mSprite, 11);

	mPhysicBody = PhysicsBody::createBox(mSprite->getContentSize()/2);
	mPhysicBody->setCollisionBitmask(BULLET_FIRE);
	mPhysicBody->setContactTestBitmask(true);
	mPhysicBody->setGravityEnable(false);
	mPhysicBody->setRotationEnable(false);
	mPhysicBody->setDynamic(false);
	//mPhysicBody->setCategoryBitmask(DYNAMIC_CATEGORY);
	mPhysicBody->setMass(300.0f);

	mSprite->setPhysicsBody(mPhysicBody);

	this->setIsDislay(false);
}

BulletFire::~BulletFire()
{

}


