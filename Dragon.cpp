#include "Model\Dragon.h"
//#include "Model\Arrow.h"
#include "cocos2d.h"
#include "Define.h"
USING_NS_CC;
Dragon::Dragon(cocos2d::Layer* layer)
{
	totalBlood = 150.0;
	currentBlood = 150.0;

	isAlive = true;

	bulletDragon = new BulletDragon(layer);

	mSprite = Sprite::createWithSpriteFrameName("dragon_00.png");
	mSprite->setPosition(Vec2(0.0f, 0.0f));
	layer->addChild(mSprite, 10);
	bloodBarBG = Sprite::create("ui/enamyBloodBarBG.png");
	bloodBarBG->setAnchorPoint(Vec2(0.5f, 0.0f));
	bloodBarBG->setPosition(Vec2(mSprite->getContentSize().width / 2, mSprite->getContentSize().height + 10));
	mSprite->addChild(bloodBarBG);

	
	bloodBar = ui::LoadingBar::create("ui/enamyBloodBar.png");
	bloodBar->setDirection(ui::LoadingBar::Direction::LEFT);
	bloodBar->setPercent(100.0f);
	bloodBar->setPosition(bloodBarBG->getContentSize() / 2);
	bloodBar->setAnchorPoint(Vec2(0.5f, 0.5f));
	bloodBarBG->addChild(bloodBar);

	mPhysicsBody = PhysicsBody::createBox(mSprite->getContentSize(), PhysicsMaterial(1.0f, 0.0f, 1.0f));
	mPhysicsBody->setCollisionBitmask(DRAGON_BISMARK);
	mPhysicsBody->setContactTestBitmask(true);
	mPhysicsBody->setDynamic(false);
	//mPhysicsBody->setCategoryBitmask(10);
	mPhysicsBody->setMass(300.0f);
	mPhysicsBody->setRotationEnable(false);

	mSprite->setPhysicsBody(mPhysicsBody);
	this->setIsDislay(false);
	
	actionOfDragon[FLY] = RepeatForever::create(getAnimate("dragon_0%d.png", 2, 0.2f));
	CC_SAFE_RETAIN(actionOfDragon[FLY]);
}
Dragon::~Dragon()
{
	actionOfDragon[FLY]->autorelease();

}

bool Dragon::Shooting(float xTarget, float yTarget)
{
	
	mSprite->stopAllActions();

	if (mPhysicsBody->getPosition().x - xTarget < 40 && mPhysicsBody->getPosition().x - xTarget > -40)
	{
		return false;
	}
	if (xTarget > this->getPosition().x)
	{
		this->turnLeft();
	}
	else
	{
		this->turnRight();
	}


	if (!bulletDragon->getIsDislay())
	{
	
		if (directionRun == TURNRIGHT)
		{
			float xArrow = mPhysicsBody->getPosition().x - bulletDragon->getContentSize().width - 5;
			float yArrow = mPhysicsBody->getPosition().y - 10;
			float deltaX = xArrow - xTarget;
			float deltaY = yArrow - yTarget;
			float distance = sqrt((deltaX*deltaX) + (deltaY*deltaY));

			float angle = (asin(deltaY / distance)*-180) / 3.14;

			bulletDragon->setPosition(xArrow, yArrow);
			bulletDragon->setIsDislay(true);

			bulletDragon->setFlippedX(false);
			bulletDragon->setRowtation(angle);

			bulletDragon->setMoveByX(deltaX / distance * 5);
			bulletDragon->setMoveByY(deltaY / distance * 5);

		}
		else
		{
			float xArrow = mPhysicsBody->getPosition().x + 45;
			float yArrow = mPhysicsBody->getPosition().y - 10;
			float deltaX = xArrow - xTarget;
			float deltaY = yArrow - yTarget;
			float distance = sqrt((deltaX*deltaX) + (deltaY*deltaY));

			float angle = (asin(deltaY / distance)*-180) / 3.14;


			bulletDragon->setPosition(xArrow, yArrow);
			bulletDragon->setIsDislay(true);

			bulletDragon->setFlippedX(true);
			bulletDragon->setRowtation(-angle);

			bulletDragon->setMoveByX(deltaX / distance * 5);
			bulletDragon->setMoveByY(deltaY / distance * 5);

			
		}

	}


	
	
	return true;
}

void Dragon::update()
{
	if (bulletDragon->getIsDislay())
	{
		bulletDragon->update();
	}
}

void Dragon::fly()
{

	if (this->getPositionY() < 400 && check2 == 0)
	{
		mSprite->setPositionY(this->getPositionY() - 1.0f);
		if (this->getPositionY() < 150)
		{
			check2 = 1;
		}
	}
	else
	{
		if (this->getPositionY() > 130 && check2 == 1)
		{
			mSprite->setPositionY(this->getPositionY() + 1.0f);
			if (this->getPositionY() > 390)
			{
				check2 = 0;
			}
		}
		
	}
	if (mSprite->getNumberOfRunningActions() <= 0)
	{
		mSprite->stopAllActions();
		mSprite->runAction(actionOfDragon[FLY]);
		actionCurren = FLY;
	}
}

void Dragon::hasCollision()
{
	bulletDragon->setIsDislay(false);
}

void Dragon::setIndex(int index)
{
	mPhysicsBody->setGroup(index);
	bulletDragon->setIndexGroup(index);
}