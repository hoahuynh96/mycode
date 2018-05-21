#include "Model\Model.h"
#include "Define.h"

USING_NS_CC;

Model::Model()
{
	mIsDislay = true;
}

Model::~Model()
{
}

void Model::setIsDislay(bool alive)
{
	mIsDislay = alive;
	mSprite->setVisible(mIsDislay);
	mPhysicsBody->setEnabled(alive);
}

bool Model::getIsDislay()
{
	return mIsDislay;
}

void Model::setPosition(cocos2d::Vec2 pos)
{
	mSprite->setPosition(pos);
}

void Model::setAnchorPoint(cocos2d::Vec2 anchor)
{
	mSprite->setAnchorPoint(anchor);
}

void Model::setScale(float scale)
{
	mSprite->setScale(scale);
}

Vec2 Model::getPosition()
{
	return mSprite->getPosition();
}

Size Model::getContentSize()
{
	return mSprite->getContentSize();
}

Animate* Model::getAnimate(const char* format, int numOfImage, float timeDelay)
{
	auto spriteFrameCache = SpriteFrameCache::getInstance();
	Vector<SpriteFrame*> listImage;
	char str[100];
	for (int i = 0; i < numOfImage; i++)
	{
		sprintf(str, format, i);
		listImage.pushBack(spriteFrameCache->getSpriteFrameByName(str));
	}

	auto animation = Animation::createWithSpriteFrames(listImage, timeDelay);

	auto animate = Animate::create(animation);

	return animate;
}

void Model::setPositionX(float x)
{
	mSprite->setPositionX(x);
}

float Model::getPositionX()
{
	return mSprite->getPositionX();
}

void Model::setPositionY(float y)
{
	mSprite->setPositionY(y);
}

float Model::getPositionY()
{
	return mSprite->getPositionY();
}

void Model::turnLeft()
{
	mSprite->setFlippedX(true);  // xoay nhan vat theo chieu nguoc lai
	directionRun = TURNLEFT;
}

void Model::turnRight()
{
	mSprite->setFlippedX(false);
	directionRun = TURNRIGHT;
}

void Model::setDirection(int direction)
{
	directionRun = direction;
}

int Model::getDirection()
{
	return directionRun;
}

void Model::setIsMove(bool isMove)
{
	this->isMove = isMove;
}

bool Model::getIsMove()
{
	return isMove;
}

int Model::getNumberOfRunningActions()
{
	return mSprite->getNumberOfRunningActions();
}

Rect Model::getBoundingBox()
{
	return mSprite->getBoundingBox();
}

void Model::setTotalBlood(float totalBlood)
{
	this->totalBlood = totalBlood;
}

float Model::getTotalBlood()
{
	return totalBlood;
}

void Model::setCurrentBlood(float currentBlood)
{
	bloodBar->setPercent(currentBlood / totalBlood * 100);
	this->currentBlood = currentBlood;
}

float Model::getCurrentBlood()
{
	return currentBlood;
}

bool Model::getIsAlive()
{
	return isAlive;
}

void Model::setIsAlive(bool isAlive)
{
	this->isAlive = isAlive;
	setIsDislay(isAlive);
}

int Model::getDamage()
{
	return damage;
}

void Model::setKeepKey(bool isKeepKey)
{
	this->isKeepKey = isKeepKey;
}

int Model::getKeepKey()
{
	return isKeepKey;
}