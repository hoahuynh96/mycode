#include "Model\Crep.h"
#include "Model\Arrow.h"
#include "cocos2d.h"
#include "Define.h"
USING_NS_CC;
Crep::Crep(cocos2d::Layer* layer)
{
	totalBlood = 200.0f;
	currentBlood = 200.0f;

	isAlive = true;

	bulletDragon = new BulletDragon(layer);

	mSprite = Sprite::createWithSpriteFrameName("crepattack_00.png");
	mSprite->setPosition(Vec2(0.0f, 0.0f));
	layer->addChild(mSprite, 13);
	mSprite->setScale(1.5f);

	bloodBarBG = Sprite::createWithSpriteFrameName("enamyBloodBarBG.png");
	bloodBarBG->setAnchorPoint(Vec2(0.5f, 0.0f));
	bloodBarBG->setScale(1 / 1.5f);
	bloodBarBG->setPosition(Vec2(mSprite->getContentSize().width / 2, mSprite->getContentSize().height + 10));
	mSprite->addChild(bloodBarBG);

	bloodBar = ui::LoadingBar::create();
	bloodBar->loadTexture("enamyBloodBar.png", ui::Widget::TextureResType::PLIST);
	bloodBar->setDirection(ui::LoadingBar::Direction::LEFT);
	bloodBar->setPercent(100.0f);
	bloodBar->setPosition(bloodBarBG->getContentSize() / 2);
	bloodBar->setAnchorPoint(Vec2(0.5f, 0.5f));
	bloodBarBG->addChild(bloodBar);
	
	mPhysicsBody = PhysicsBody::createBox(mSprite->getContentSize(), PhysicsMaterial(1.0f, 0.0f, 1.0f));
	mPhysicsBody->setCollisionBitmask(CREP_BISMARK);
	mPhysicsBody->setContactTestBitmask(true);
	mPhysicsBody->setDynamic(true);
	//mPhysicsBody->setCategoryBitmask(10);
	mPhysicsBody->setMass(300.0f);
	mPhysicsBody->setRotationEnable(false);

	mSprite->setPhysicsBody(mPhysicsBody);
	this->setIsDislay(false);
	mSprite->setFlippedX(true);

	scopeDamage = Node::create();
	scopeDamage->setContentSize(Size(10.0f, 100.0f));
	scopeDamage->setPosition(mSprite->getContentSize() / 8);
	scopeDamage->setAnchorPoint(Vec2(0.5f, 0.0f));
	mSprite->addChild(scopeDamage);

	scopeDamageBody = PhysicsBody::createBox(Size(10.0f, 20.0f));
	scopeDamageBody->setCollisionBitmask(CREP_DAMAGE_BISMARK);
	scopeDamageBody->setContactTestBitmask(true);
	scopeDamageBody->setGravityEnable(false);
	scopeDamageBody->setDynamic(false);
	scopeDamage->setPhysicsBody(scopeDamageBody);

	scopeDamage->setVisible(false);
	scopeDamageBody->setEnabled(false);
	actionOfCrep[GO_CREP] = Repeat::create(getAnimate("creprun_0%d.png", 5, 0.2f),1);
	CC_SAFE_RETAIN(actionOfCrep[GO_CREP]);
	actionOfCrep[ACTACK_CREP] = Repeat::create(getAnimate("crepattack_0%d.png", 4, 0.2f),1);
	CC_SAFE_RETAIN(actionOfCrep[ACTACK_CREP]);
	
}
Crep::~Crep()
{
	actionOfCrep[GO_CREP]->autorelease();
	actionOfCrep[ACTACK_CREP]->autorelease();
}

//void Crep::update()
//{
//	if (bulletDragon->getIsDislay())
//	{
//		bulletDragon->update();
//	}
//}

void Crep::actack()
{

	if (actionCurren == ACTACK_CREP && mSprite->getNumberOfRunningActions() > 0)
	{

	}
	else
	{
		dame();
		mSprite->stopAllActions();
		mSprite->runAction(actionOfCrep[ACTACK_CREP]);
		actionCurren = ACTACK_CREP;
	}

}
void Crep::dame()
{
	if (this->getIsAlive())
	{
		scopeDamageBody->setEnabled(true);


		MoveBy* moveby;

		if (directionRun == TURNLEFT)
		{

			moveby = MoveBy::create(0.4f, Vec2(-60.0f, 0));

		}
		else
		{
			moveby = MoveBy::create(0.4f, Vec2(+120.0f, 0));
		}

		auto finishAttack = CallFunc::create([&]()
		{
			scopeDamageBody->setEnabled(false);
			scopeDamage->setRotation(0.0f);
			scopeDamage->setPosition(mSprite->getContentSize() / 8);

		});

		auto sequence = Sequence::create(moveby, finishAttack, nullptr);

		scopeDamage->runAction(sequence);
	}
	
}

void Crep::runCrep(float x, float y)
{
	float x1 = this->getPosition().x;
	float y1 = this->getPosition().y;

	if (x < x1)
	{
		this->turnLeft();
		if (x - x1 <= this->getContentSize().width)
		{
			this->setPositionX(this->getPositionX() - 1.5);
		}


	}
	else
	{
		this->turnRight();
		if (x - x1 >= this->getContentSize().width)
		{
			this->setPositionX(this->getPositionX() + 1.5);
		}

	}
	if (mSprite->getNumberOfRunningActions() <= 0)
	{
		mSprite->stopAllActions();
		mSprite->runAction(actionOfCrep[GO_CREP]);
		actionCurren = GO_CREP;
	}

}

void Crep::hasCollision()
{
	bulletDragon->setIsDislay(false);
}

void Crep::setIndex(int index)
{
	mPhysicsBody->setGroup(index);
	bulletDragon->setIndexGroup(index);
}