#include "Model\Mercenary.h"
#include "Model\GameSetting.h"
#include "Define.h"

USING_NS_CC;

Mercenary::Mercenary(TMXTiledMap* layer)
{
	GameSetting* gameSetting = GameSetting::getInstance();

	currentAction = MERCENARY_RUN;
	directionRun = TURNRIGHT;
	totalBlood = 30 + (gameSetting->getLevel() - 1)*15.0f;
	currentBlood = 30 + (gameSetting->getLevel() - 1)*15.0f;
	damage = 15 + (gameSetting->getLevel() - 1)*8.0f;
	isAlive = true;
	isKeepKey = false;

	mSprite = Sprite::createWithSpriteFrameName("Mercenary_run_00.png");
	mSprite->setAnchorPoint(Vec2(0.5f, 0.0f));
	mSprite->setScale(0.8f);
	layer->addChild(mSprite, 11);

	mPhysicsBody = PhysicsBody::createBox(mSprite->getContentSize(), PhysicsMaterial(1000.0f, 0.0f, 1.0f));
	mPhysicsBody->setCollisionBitmask(MERCENARY_BISKMARK);
	mPhysicsBody->setContactTestBitmask(true);
	//mPhysicsBody->setCategoryBitmask(DYNAMIC_CATEGORY);
	mSprite->setPhysicsBody(mPhysicsBody);

	bloodBarBG = Sprite::createWithSpriteFrameName("enamyBloodBarBG.png");
	bloodBarBG->setAnchorPoint(Vec2(0.5f, 0.0f));
	bloodBarBG->setPosition(Vec2(mSprite->getContentSize().width / 2, mSprite->getContentSize().height + 10));
	mSprite->addChild(bloodBarBG);

	bloodBar = ui::LoadingBar::create();
	bloodBar->loadTexture("enamyBloodBar.png", ui::Widget::TextureResType::PLIST);
	bloodBar->setDirection(ui::LoadingBar::Direction::LEFT);
	bloodBar->setPercent(100.0f);
	bloodBar->setPosition(bloodBarBG->getContentSize() / 2);
	bloodBar->setAnchorPoint(Vec2(0.5f, 0.5f));

	bloodBarBG->addChild(bloodBar);

	scopeDamage = Node::create();
	scopeDamage->setContentSize(Size(10.0f, 120.0f));
	scopeDamage->setPosition(mSprite->getContentSize() / 2);
	scopeDamage->setAnchorPoint(Vec2(0.5f, 0.0f));
	scopeDamage->setRotation(0.0f);
	mSprite->addChild(scopeDamage);

	scopeDamageBody = PhysicsBody::createBox(Size(2.0f, 20.0f));
	scopeDamageBody->setCollisionBitmask(MERCENARY_DAME_BISMARK);
	scopeDamageBody->setContactTestBitmask(true);
	scopeDamageBody->setGravityEnable(false);
	//scopeDamageBody->setCategoryBitmask(DYNAMIC_CATEGORY);
	scopeDamageBody->setDynamic(false);
	scopeDamage->setPhysicsBody(scopeDamageBody);

	scopeDamage->setVisible(false);
	scopeDamageBody->setEnabled(false);

	actionOfMercenary[MERCENARY_RUN] = Repeat::create(getAnimate("Mercenary_run_0%d.png", 5, 0.1f), 1);
	actionOfMercenary[MERCENARY_ATTACK] = Repeat::create(getAnimate("Mercenary_attack_0%d.png", 5, 0.2f),1);
	actionOfMercenary[MERCENARY_FIRE] = Repeat::create(getAnimate("Mercenary_fire_0%d.png", 5, 0.2f), 1);

	CC_SAFE_RETAIN(actionOfMercenary[MERCENARY_RUN]);
	CC_SAFE_RETAIN(actionOfMercenary[MERCENARY_ATTACK]);
	CC_SAFE_RETAIN(actionOfMercenary[MERCENARY_FIRE]);

	bulletFire = new BulletFire(layer);

	this->setIsDislay(false);
}

Mercenary::~Mercenary()
{
	for (int i = 0; i < 3; i++)
	{
		actionOfMercenary[i]->autorelease();
	}
}

int Mercenary::getDamage()
{
	return damage;
}

void Mercenary::Run()
{
	if (mSprite->getNumberOfRunningActions() <= 0)
	{
		mSprite->runAction(actionOfMercenary[MERCENARY_RUN]);
		currentAction = MERCENARY_RUN;
	}
}

void Mercenary::Attack()
{
	if (mSprite->getNumberOfRunningActions() <= 0)
	{
		Damage();
		mSprite->runAction(actionOfMercenary[MERCENARY_ATTACK]);
		currentAction = MERCENARY_ATTACK;
	}
}

void Mercenary::Fire()
{
	if (!bulletFire->getIsDislay())
	{
		if (mSprite->getNumberOfRunningActions() <= 0)
		{
			mSprite->runAction(actionOfMercenary[MERCENARY_FIRE]);
			currentAction = MERCENARY_FIRE;

			bulletFire->setIsDislay(true);

			bulletFire->setMoveByY(0.0f);
			if (directionRun == TURNRIGHT)
			{
				bulletFire->setFlippedX(false);
				bulletFire->setMoveByX(-6.0f);
				bulletFire->setPosition(mSprite->getPositionX() + mSprite->getContentSize().width + 4.0f, mSprite->getPositionY() + 35.0f);
			}
			else
			{
				bulletFire->setFlippedX(true);
				bulletFire->setMoveByX(6.0f);
				bulletFire->setPosition(mSprite->getPositionX() - mSprite->getContentSize().width*0.6f, mSprite->getPositionY() + 35.0f);
			}
		}
	}
	
}

void Mercenary::setIndex(int index)
{
	mPhysicsBody->setGroup(index);
	bulletFire->setIndexGroup(index);
}

void Mercenary::setMinPosition(int minPosition)
{
	this->minPosition = minPosition;
}

int Mercenary::getMinPosition()
{
	return minPosition;
}

void Mercenary::setMaxPosition(int maxPosition)
{
	this->maxPosition = maxPosition;
}

int Mercenary::getMaxPosition()
{
	return maxPosition;
}

void Mercenary::Update()
{

	if (directionRun == TURNRIGHT)
	{
		mSprite->setPositionX(mSprite->getPositionX() + 2);
	}
	else
	{
		mSprite->setPositionX(mSprite->getPositionX() - 2);
	}
	Run();
}

void Mercenary::autoWalk()
{
	if (mSprite->getPositionX() <= minPosition)
	{
		this->turnRight();
	}
	else if (mSprite->getPositionX() >= maxPosition)
	{
		this->turnLeft();
	}

	Update();
}

void Mercenary::chase(float x)
{

	if (x - mSprite->getPositionX() < 300.0f && x - mSprite->getPositionX() > -300)
	{
		if (x < mSprite->getPositionX())
		{
			this->turnLeft();
		}
		else
		{
			this->turnRight();
		}


		if (x - mSprite->getPositionX() < 70.0f && x - mSprite->getPositionX() > -55.0f)
		{
			Attack();
		}
		else
		{
			Fire();
		}
	}
	else
	{
		autoWalk();
	}
}

void Mercenary::Damage()
{
	scopeDamageBody->setEnabled(true);

	RotateTo* rotation;

	if (directionRun == TURNLEFT)
	{
		rotation = RotateTo::create(0.6f, -120);
	}
	else
	{
		rotation = RotateTo::create(0.6f, 120);
	}

	auto finishAttack = CallFunc::create([&]()
	{
		scopeDamageBody->setEnabled(false);
		scopeDamage->setRotation(0.0f);
	});

	auto sequence = Sequence::create(rotation, finishAttack, nullptr);

	scopeDamage->runAction(sequence);
}

void Mercenary::hasCollistion()
{
	bulletFire->setIsDislay(false);
}

void Mercenary::updateFire()
{
	if (bulletFire->getIsDislay())
	{
		bulletFire->update();
	}
}