#include "Model\Kunai.h"
#include "Define.h"

USING_NS_CC;

Kunai::Kunai(Layer* layer)
{
	damage = 10;

	mSprite = Sprite::createWithSpriteFrameName("kunai.png");
	mSprite->setPosition(Vec2(-100.0f, -100.0f));
	layer->addChild(mSprite,10);

	mPhysicBody = PhysicsBody::createCircle(14, PhysicsMaterial(1.0f, 0.0f, 1.0f));
	mPhysicBody->setCollisionBitmask(KUNAI_BISMARK);
	mPhysicBody->setContactTestBitmask(true);
	mPhysicBody->setGravityEnable(false);
	//mPhysicBody->setCategoryBitmask(DYNAMIC_CATEGORY);
	mPhysicBody->setMass(300.0f);

	mSprite->setPhysicsBody(mPhysicBody);

	this->setIsDislay(false);
}

Kunai::~Kunai()
{

}