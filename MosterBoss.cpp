#include"Model\MosterBoss.h"
#include"Define.h"
#include"cocos2d.h"
USING_NS_CC;

MosterBoss::MosterBoss(cocos2d::Layer* layer)
{
	

	currentBlood = 300.0f;
	totalBlood = 300.0f;

	mSprite = Sprite::createWithSpriteFrameName("run_00.png");
	mSprite->setAnchorPoint(Vec2(0.0f, 0.0f));
	mSprite->setFlipX(true);
	directionRun = TURNLEFT;
	layer->addChild(mSprite, 3);

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

	mPhysicsBody = PhysicsBody::createBox(mSprite->getContentSize(), PhysicsMaterial(0.0f, 0.0f, 1.0f));
	mPhysicsBody->setRotationEnable(false);
	mPhysicsBody->setMass(500);
	mPhysicsBody->setCollisionBitmask(BOSS_BISMARK);
	mPhysicsBody->setContactTestBitmask(true);
	mPhysicsBody->setAngularVelocityLimit(20);
	mSprite->setPhysicsBody(mPhysicsBody);

	scopeDamage = Node::create();
	scopeDamage->setContentSize(Size(10.0f, 100.0f));
	scopeDamage->setPosition(mSprite->getContentSize()/8 );
	scopeDamage->setAnchorPoint(Vec2(0.5f, 0.0f));
	mSprite->addChild(scopeDamage);

	scopeDamageBody = PhysicsBody::createBox(Size(10.0f, 20.0f));
	scopeDamageBody->setCollisionBitmask(MONTER_DAMAGE_BISMARK);
	scopeDamageBody->setContactTestBitmask(true);
	scopeDamageBody->setGravityEnable(false);
	scopeDamageBody->setDynamic(false);
	scopeDamage->setPhysicsBody(scopeDamageBody);

	scopeDamage->setVisible(false);
	scopeDamageBody->setEnabled(false);
	

	lisAction[RUN_BOSS] = Repeat::create(getAnimate("run_0%d.png", 6, 0.1f), 1);
	lisAction[ATTACK_BOSS] = Repeat::create(getAnimate("acttack_0%d.png", 4, 0.2f), 1);
	lisAction[SKILL_BOSS] = Repeat::create(getAnimate("Skill_0%d.png", 3, 0.5f), 1);	
	lisAction[RUNBACK_BOSS] = Repeat::create(getAnimate("runback_0%d.png", 6, 0.1f), 1);
	

	CC_SAFE_RETAIN(lisAction[RUN_BOSS]);
	CC_SAFE_RETAIN(lisAction[ATTACK_BOSS]);
	CC_SAFE_RETAIN(lisAction[SKILL_BOSS]);
	CC_SAFE_RETAIN(lisAction[RUNBACK_BOSS]);


	
}
MosterBoss::~MosterBoss()
{
	for (int i = 0; i < 4; i++)
	{
		lisAction[i]->autorelease();
	}
	
}
void MosterBoss::runBoss(float x, float y)
{
	
	float x1 = this->getPosition().x;
	float y1 = this->getPosition().y;

	if (x < x1)
	{
		this->turnLeft();
		if (x - x1 <= this->getContentSize().width )
		{
			this->setPositionX(this->getPositionX() - 1.5);
		}
		

	}
	else
	{
		this->turnRight();
		if (x - x1 >= this->getContentSize().width )
		{
			this->setPositionX(this->getPositionX() + 1.5);
		}
		
	}
	

	if (mSprite->getNumberOfRunningActions() <= 0)
	{
		mSprite->stopAllActions();
		mSprite->runAction(lisAction[RUN_BOSS]);
		actionCurren = RUN_BOSS;
	}

	
}
void MosterBoss::runback()
{
	
	if (directionRun == TURNLEFT)
	{
		
		if (mSprite->getPositionX() < 600)
		{
			//mSprite->setPosition((Vec2(mSprite->getPositionX() + 150.0f,mSprite->getPositionY()+150.0f)));
			mSprite->setPositionX((mSprite->getPositionX() + 1.5f));
			mSprite->setVisible(true);
		}
		
		
		
			
	}
	else
	{
		
	}
	
	if (actionCurren == RUNBACK_BOSS && mSprite->getNumberOfRunningActions() > 0)
	{

	}
	else
	{
		mSprite->stopAllActions();
		mSprite->runAction(lisAction[RUNBACK_BOSS]);
		actionCurren = RUNBACK_BOSS;

	}
	
}
void MosterBoss::actack()
{

	if (actionCurren == ATTACK_BOSS && mSprite->getNumberOfRunningActions() > 0)
	{
		
	}
	else
	{
		dame();
		mSprite->stopAllActions();
		mSprite->runAction(lisAction[ATTACK_BOSS]);
		actionCurren = ATTACK_BOSS;
	}


	
	
}


void MosterBoss::skill()
{
	
	if (actionCurren == SKILL_BOSS && mSprite->getNumberOfRunningActions() > 0)
			{
				
			}
			else
			{
				
				mSprite->stopAllActions();
				mSprite->runAction(lisAction[SKILL_BOSS]);
				actionCurren = SKILL_BOSS;
		
			}
}

void MosterBoss::kich(float x)
{
	float x1 = mSprite->getPositionX();

	if (x < x1)
	{
		this->turnLeft();
	}
	else
	{
		this->turnRight();
	}
	if (actionCurren == KICH && mSprite->getNumberOfRunningActions() > 0)
	{

	}
	else
	{
		mSprite->stopAllActions();
		mSprite->runAction(lisAction[KICH]);
		actionCurren = KICH;
	}
}

void MosterBoss::dame()
{
	scopeDamageBody->setEnabled(true);

	
	MoveBy* moveby;

	if (directionRun == TURNLEFT)
	{
		
		moveby = MoveBy::create(0.4f, Vec2(-60.0f,0));
		
	}
	else
	{
		moveby = MoveBy::create(0.4f, Vec2(+120.0f, 0));
	}

	auto finishAttack = CallFunc::create([&]()
	{
		scopeDamageBody->setEnabled(false);
		scopeDamage->setRotation(0.0f);
		scopeDamage->setPosition(mSprite->getContentSize()/8 );

	});

	auto sequence = Sequence::create(moveby, finishAttack, nullptr);

	scopeDamage->runAction(sequence);
}
void MosterBoss::enable()
{
	mSprite->setVisible(false);
}
void MosterBoss::blood_loss(float dame)
{
	this->setCurrentBlood(this->getCurrentBlood() - dame);

}

