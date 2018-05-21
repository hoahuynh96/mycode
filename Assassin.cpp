#include "Model\Assassin.h"
#include "Model\GameSetting.h"
#include "Define.h"

USING_NS_CC;

Assassin::Assassin(cocos2d::TMXTiledMap* layer)
{
	GameSetting* gameSetting = GameSetting::getInstance();
	currentAction = IDLE;
	currentBlood = 50.0f + (gameSetting->getLevel() - 1) * 10.0f;
	totalBlood = 50.0f + (gameSetting->getLevel() - 1) * 10.0f;
	damage = 10 + (gameSetting->getLevel() - 1)*5.0f;
	directionRun = TURNRIGHT;
	isKeepKey = false;

	isAlive = true;

	mSprite = Sprite::createWithSpriteFrameName("assassin_walk_00.png");
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

	mPhysicsBody = PhysicsBody::createBox(Size(mSprite->getContentSize().width/2, mSprite->getContentSize().height), PhysicsMaterial(1000.0f, 0.0f, 1.0f));
	mPhysicsBody->setCollisionBitmask(ASSASSIN_BISMARK);
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

	listAction[IDLE] = Repeat::create(getAnimate("assassin_walk_0%d.png", 6, 0.15f),1);
	listAction[ATTACK] = Repeat::create(getAnimate("assassin_attack_0%d.png", 6, 0.15f), 1);

	//retain
	CC_SAFE_RETAIN(listAction[IDLE]);
	CC_SAFE_RETAIN(listAction[ATTACK]);

}

Assassin::~Assassin()
{
	for (int i = 0; i < 2; i++)
	{
		listAction[i]->autorelease();
	}
}

void Assassin::Attack()
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

void Assassin::Idel()
{
	if (mSprite->getNumberOfRunningActions() <= 0)
	{
		mSprite->stopAllActions();
		mSprite->runAction(listAction[IDLE]);
		currentAction = IDLE;
	}
}

void Assassin::setIndex(int index)
{
	mPhysicsBody->setGroup(index);
}

void Assassin::Damage()
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

void Assassin::Update()
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

void Assassin::setMinPosition(int minPosition)
{
	this->minPosition = minPosition;
}

int Assassin::getMinPosition()
{
	return minPosition;
}

void Assassin::setMaxPosition(int maxPosition)
{
	this->maxPosition = maxPosition;
}

int Assassin::getMaxPosition()
{
	return maxPosition;
}

void Assassin::autoWalk()
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

void Assassin::chase(float x)
{
	if (x < mSprite->getPositionX())
	{
		this->turnLeft();
	}
	else if (x > mSprite->getPositionX())
	{
		this->turnRight();
	}

	Update();
}