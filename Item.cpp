#include "Model\Item.h"
#include "Define.h"

USING_NS_CC;

Item::Item(TMXTiledMap* tileMap, char nameSprite[50], int type)
{
	mSprite = Sprite::createWithSpriteFrameName(nameSprite);
	mSprite->setPosition(Vec2(-500.0f, -500.0f));
	mSprite->setAnchorPoint(Vec2(0.0f, 0.0f));
	tileMap->addChild(mSprite, 11);

	switch (type)
	{
	case COINS_ITEM:
		valueItem = 10;
		mSprite->setScale(0.2f);
		mPhysicsBody = PhysicsBody::createCircle(mSprite->getContentSize().width / 2);
		mPhysicsBody->setContactTestBitmask(true);
		mPhysicsBody->setCollisionBitmask(COINS_BISMARK);
		mPhysicsBody->setMass(30.0f);
		
		//mPhysicsBody->setCategoryBitmask(ITEM_CATEGORY);
		break;
	case KEY_ITEM:
		valueItem = 0;
		mSprite->setScale(0.5f);
		mPhysicsBody = PhysicsBody::createBox(mSprite->getContentSize());
		mPhysicsBody->setContactTestBitmask(true);
		mPhysicsBody->setCollisionBitmask(KEY_BISMARK);
		mPhysicsBody->setMass(30.0f);
	}
	
	mSprite->setPhysicsBody(mPhysicsBody);

	typeItem = type;


	this->setIsDislay(false);

}

Item::~Item()
{

}

int Item::getValue()
{
	return valueItem;
}

void Item::setValue(int valueItem)
{
	this->valueItem = valueItem;
}

void Item::setPosition(float x, float y)
{
	mSprite->setPosition(Vec2(x, y));
}

void Item::setIsDislay(bool isDislay)
{
	mSprite->setVisible(isDislay);
	this->isDislay = isDislay;
	mPhysicsBody->setEnabled(isDislay);
}

bool Item::getIsDislay()
{
	return isDislay;
}

void Item::setGroupIndex(int index)
{
	mPhysicsBody->setGroup(index);
}

int Item::getTypeItem()
{
	return typeItem;
}

void Item::monneyAnimation(cocos2d::Action* action)
{
	mSprite->stopAllActions();
	mSprite->runAction(action);
	mPhysicsBody->setVelocity(Vec2(0.0f, 340.0f));

}