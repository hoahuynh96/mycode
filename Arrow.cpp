#include "Model\Arrow.h"

#include "Define.h"

USING_NS_CC;

Arrow::Arrow(cocos2d::TMXTiledMap* layer)
{
	
	mSprite = Sprite::create("arrow.png");

	mPhysicBody = PhysicsBody::createBox(mSprite->getContentSize(), PhysicsMaterial(1.0f, 0.0f, 1.0f));
	mPhysicBody->setGravityEnable(false);
	//mPhysicBody->setRotationEnable(false);
	mPhysicBody->setDynamic(false);
	mPhysicBody->setCollisionBitmask(ARROW_BISMARK);
	//mPhysicBody->setCategoryBitmask(DYNAMIC_CATEGORY);
	mPhysicBody->setMass(100);
	mPhysicBody->setContactTestBitmask(true);

	mSprite->setPhysicsBody(mPhysicBody);

	layer->addChild(mSprite,1);
	this->setIsDislay(false);
}
Arrow::~Arrow()
{

}

void Arrow::setRowtation(float angle)
{
	mSprite->setRotation(angle);
}