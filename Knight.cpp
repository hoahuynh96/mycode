#include "Model\Knight.h"
#include "Model\GameSetting.h"
#include "Define.h"

USING_NS_CC;

Knight::Knight(cocos2d::TMXTiledMap* layer)
{
	GameSetting* gameSetting = GameSetting::getInstance();
	currentAction = IDLE;
	currentBlood = 40.0f + (gameSetting->getLevel() - 1) * 15.0f;
	totalBlood = 40.0f + (gameSetting->getLevel() - 1) * 15.0f;
	damage = 20 + (gameSetting->getLevel() - 1)*8.0f;
	directionRun = TURNRIGHT;
	isKeepKey = false;

	isAlive = true;

	mSprite = Sprite::createWithSpriteFrameName("_RUN_000.png");
	mSprite->setAnchorPoint(Vec2(0.5f, 0.0f));
	mSprite->setScale(0.8f);
	layer->addChild(mSprite, 8);

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

	mPhysicsBody = PhysicsBody::createBox(Size(mSprite->getContentSize().width / 2, mSprite->getContentSize().height), PhysicsMaterial(1000.0f, 0.0f, 1.0f));
	mPhysicsBody->setCollisionBitmask(KNIGHT_BISMARK);
	mPhysicsBody->setContactTestBitmask(true);
	//mPhysicsBody->setCategoryBitmask(DYNAMIC_CATEGORY);
	mPhysicsBody->setMass(800);
	mPhysicsBody->setRotationEnable(false);
	mSprite->setPhysicsBody(mPhysicsBody);

	this->setIsDislay(false);

	scopeDamage = Node::create();
	scopeDamage->setContentSize(Size(10.0f, 120.0f));
	scopeDamage->setPosition(mSprite->getContentSize() / 2);
	scopeDamage->setAnchorPoint(Vec2(0.5f, 0.0f));
	scopeDamage->setRotation(180.0f);
	mSprite->addChild(scopeDamage);

	scopeDamageBody = PhysicsBody::createBox(Size(10.0f, 20.0f));
	scopeDamageBody->setCollisionBitmask(MONTER_DAMAGE_BISMARK);
	scopeDamageBody->setContactTestBitmask(true);
	scopeDamageBody->setGravityEnable(false);
	scopeDamageBody->setDynamic(false);
	//scopeDamageBody->setCategoryBitmask(DYNAMIC_CATEGORY);
	scopeDamage->setPhysicsBody(scopeDamageBody);

	scopeDamage->setVisible(false);
	scopeDamageBody->setEnabled(false);

	listAction[IDLE] = Repeat::create(getAnimate("_RUN_00%d.png", 7, 0.15f), 1);
	listAction[ATTACK] = Repeat::create(getAnimate("_ATTACK_00%d.png", 7, 0.15f), 1);

	//retain
	CC_SAFE_RETAIN(listAction[IDLE]);
	CC_SAFE_RETAIN(listAction[ATTACK]);

}

Knight::~Knight()
{
	for (int i = 0; i < 2; i++)
	{
		listAction[i]->autorelease();
	}
}

void Knight::Attack()
{
	if (currentAction == ATTACK && mSprite->getNumberOfRunningActions() > 0)
	{

	}
	else
	{
		Damage();

		mSprite->stopAllActions();
		mSprite->runAction(listAction[ATTACK]);
		currentAction = ATTACK;
	}
}

void Knight::Idel()
{
	if (mSprite->getNumberOfRunningActions() <= 0)
	{
		mSprite->stopAllActions();
		mSprite->runAction(listAction[IDLE]);
		currentAction = IDLE;
	}
}

void Knight::setIndex(int index)
{
	mPhysicsBody->setGroup(index);
}

void Knight::Damage()
{
	scopeDamageBody->setEnabled(true);

	RotateTo* rotation;

	if (directionRun == TURNLEFT)
	{
		rotation = RotateTo::create(0.6f, -60);
	}
	else
	{
		rotation = RotateTo::create(0.6f, 60);
	}

	auto finishAttack = CallFunc::create([&]()
	{
		scopeDamageBody->setEnabled(false);
		scopeDamage->setRotation(180.0f);
	});

	auto sequence = Sequence::create(rotation, finishAttack, nullptr);

	scopeDamage->runAction(sequence);
}

void Knight::Update()
{
	if (currentAction == ATTACK && mSprite->getNumberOfRunningActions() > 0)
	{

	}
	else
	{
		if (directionRun == TURNRIGHT)
		{
			mSprite->setPositionX(mSprite->getPositionX() + 1);
		}
		else
		{
			mSprite->setPositionX(mSprite->getPositionX() - 1);
		}

		Idel();
	}

}

void Knight::setMinPosition(int minPosition)
{
	this->minPosition = minPosition;
}

int Knight::getMinPosition()
{
	return minPosition;
}

void Knight::setMaxPosition(int maxPosition)
{
	this->maxPosition = maxPosition;
}

int Knight::getMaxPosition()
{
	return maxPosition;
}

void Knight::autoWalk()
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

void Knight::chase(float x)
{
	if (x < mSprite->getPositionX())
	{
		this->turnLeft();
	}
	else
	{
		this->turnRight();
	}

	Update();
}