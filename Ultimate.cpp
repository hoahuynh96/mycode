#include"Ultimate.h"
#include "Define.h"
#include"cocos2d.h"
USING_NS_CC;

Ultimatel::Ultimatel(cocos2d::Layer* layer)
{
	mSprite = Sprite::create("fire_00.png");
	mPhysicsBody = PhysicsBody::createBox(mSprite->getContentSize(), PhysicsMaterial(0.0f, 0.0f, 1.0f));
	mPhysicsBody->setCollisionBitmask(ULTIDRAGON_BISMARK);
	mPhysicsBody->setContactTestBitmask(true);
	mPhysicsBody->setMass(0.01f);
	mPhysicsBody->setRotationEnable(false);
	mPhysicsBody->setDynamic(true);
	mSprite->setPhysicsBody(mPhysicsBody);
	
	layer->addChild(mSprite, 12);
	this->setIsDislay(false);

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/fire.plist");
	lisAction[FALL_FIRE] = RepeatForever::create(getAnimate("fire_0%d.png", 4, 0.1f));
	CC_SAFE_RETAIN(lisAction[FALL_FIRE]);
}
Ultimatel::~Ultimatel()
{
	
}
void Ultimatel::fall()
{

	float y = mSprite->getPositionY();
	if (y < 0 - mSprite->getContentSize().height / 2)
	{
	
		this->setIsDislay(false);
		mSprite->setPositionY(200);
	}
	else
	{
		this->setIsDislay(true);
	}

	if (actionCurren == FALL_FIRE && mSprite->getNumberOfRunningActions() > 0)
	{
	}
	else
	{
		mSprite->stopAllActions();
		mSprite->runAction(lisAction[FALL_FIRE]);
		actionCurren = FALL_FIRE;

	}
}

