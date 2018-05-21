#include "Model\Player.h"
#include "Model\GameSetting.h"
//#include "SimpleAudioEngine.h"
#include "ui\CocosGUI.h"

USING_NS_CC;

Player::Player(Layer* layer)
{
	GameSetting* gameSetting = GameSetting::getInstance();

	currentAction = IDLE;
	directionRun = TURNRIGHT;
	currentBlood = gameSetting->getBloodPlayer(); 
	totalBlood = gameSetting->getBloodPlayer();
	isMove = false;
	isOnLand = true;
	preventLeft = false;
	preventRight = false;
	armor = gameSetting->getArmor();
	damage = gameSetting->getDamgePlayer();

	bloodBarBG = Sprite::createWithSpriteFrameName("hubBG.png");
	bloodBarBG->setAnchorPoint(Vec2(0.0f, 1.0f));
	bloodBarBG->setPosition(Vec2(10.0f, SCENE_H - 10.0f));
	
	layer->addChild(bloodBarBG, 10);

	bloodBar = ui::LoadingBar::create();
	bloodBar->loadTexture("player_bloodBar.png",ui::Widget::TextureResType::PLIST);
	bloodBar->setDirection(ui::LoadingBar::Direction::LEFT);
	bloodBar->setPercent(100.0f);
	bloodBar->setPosition(Vec2(86.0f, 44.0f));
	bloodBar->setAnchorPoint(Vec2(0.0f, 0.0f));

	bloodBarBG->addChild(bloodBar, 11);

	mSprite = Sprite::createWithSpriteFrameName("Idle__000.png");
	mSprite->setPosition(Vec2(0.0f, 0.0f));
	layer->addChild(mSprite, 3);

	scopeDamage = Node::create();
	scopeDamage->setContentSize(Size(10.0f, 120.0f));
	scopeDamage->setPosition(mSprite->getContentSize() / 2);
	scopeDamage->setAnchorPoint(Vec2(0.5f, 0.0f));

	mSprite->addChild(scopeDamage);

	scopeDamageBody = PhysicsBody::createBox(Size(10.0f, 20.0f));
	scopeDamageBody->setCollisionBitmask(PLAYER_DAMAGE_BISMARK);
	scopeDamageBody->setContactTestBitmask(true);
	scopeDamageBody->setGravityEnable(false);
	//scopeDamageBody->setCategoryBitmask(DYNAMIC_CATEGORY);
	scopeDamageBody->setDynamic(false);

	scopeDamage->setPhysicsBody(scopeDamageBody);

	scopeDamage->setVisible(false);
	scopeDamageBody->setEnabled(false);

	for (int i = 0; i < 10; i++)
	{
		Kunai* newKunai = new Kunai(layer);
		kunais.push_back(newKunai);
	}

	powerSword = new PowerSword(layer);

	mPhysicsBody = PhysicsBody::createBox(mSprite->getContentSize(), PhysicsMaterial(100.0f, 0.0f, 1.0f));
	mPhysicsBody->setRotationEnable(false);
	mPhysicsBody->setMass(500.0f);
	mPhysicsBody->setCollisionBitmask(PLAYER_BISMARK);
	mPhysicsBody->setContactTestBitmask(true);
	//mPhysicsBody->setCategoryBitmask(DYNAMIC_CATEGORY);
	mPhysicsBody->setDynamic(true);
	mSprite->setPhysicsBody(mPhysicsBody);

	actionOfPlayer[IDLE] = Repeat::create(getAnimate("Idle__00%d.png", 10, 0.1f), 1);
	actionOfPlayer[ATTACK] = Repeat::create(getAnimate("Attack__00%d.png", 10, 0.07f), 1);
	actionOfPlayer[JUMP] = Repeat::create(getAnimate("Jump__00%d.png", 10, 0.1f), 1);
	actionOfPlayer[RUN] = Repeat::create(getAnimate("Run__00%d.png", 10, 0.1f), 1);
	actionOfPlayer[THROWKUNAI] = Repeat::create(getAnimate("Throw__00%d.png", 10, 0.05f), 1);
	actionOfPlayer[SLIDE] = Repeat::create(getAnimate("Slide__00%d.png", 10, 0.05f), 1);
	actionOfPlayer[JUMPTHROW] = Repeat::create(getAnimate("Jump_Throw__00%d.png", 10, 0.05f), 1);
	actionOfPlayer[JUMPATTACK] = Repeat::create(getAnimate("Jump_Attack__00%d.png", 10, 0.07f), 1);

	//retain
	CC_SAFE_RETAIN(actionOfPlayer[RUN]);
	CC_SAFE_RETAIN(actionOfPlayer[JUMP]);
	CC_SAFE_RETAIN(actionOfPlayer[ATTACK]);
	CC_SAFE_RETAIN(actionOfPlayer[IDLE]);
	CC_SAFE_RETAIN(actionOfPlayer[THROWKUNAI]);
	CC_SAFE_RETAIN(actionOfPlayer[SLIDE]);
	CC_SAFE_RETAIN(actionOfPlayer[JUMPTHROW]);
	CC_SAFE_RETAIN(actionOfPlayer[JUMPATTACK]);

}

Player::~Player()
{
	CC_SAFE_DELETE(mSprite);

	for (int i = 0; i < 4; i++)
	{
		if (actionOfPlayer[i])
		{
			actionOfPlayer[i]->autorelease();
		}
	}

}

void Player::Idle()
{

	mSprite->stopAllActions();
	mSprite->runAction(actionOfPlayer[IDLE]);
	currentAction = IDLE;

}

void Player::Jump()
{

	if (isOnLand && currentAction != SLIDE)
	{
		if (GameSetting::getInstance()->isEnableSound())
		{
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/JumpPlayer.mp3", false);
		}

		mSprite->stopAllActions();
		mSprite->runAction(actionOfPlayer[JUMP]);
		currentAction = JUMP;
		isOnLand = false;
		mPhysicsBody->setVelocity(Vec2(0.0f, 400.0f));
	}
}

void Player::Run()
{
	if (isOnLand == false || currentAction == RUN && mSprite->getNumberOfRunningActions() > 0)
	{

	}
	else
	{
		mSprite->stopAllActions();
		mSprite->runAction(actionOfPlayer[RUN]);
		currentAction = RUN;
	}

}

void Player::Attack()
{
	if (currentAction == JUMP)
	{
		if (GameSetting::getInstance()->isEnableSound())
		{
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/Chem.wav", false);
		}

		Damage();

		mSprite->stopAllActions();
		mSprite->runAction(actionOfPlayer[JUMPATTACK]);
		currentAction = JUMPATTACK;
	}
	else if (currentAction == RUN || currentAction == IDLE)
	{
		if (GameSetting::getInstance()->isEnableSound())
		{
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/Chem.wav", false);
		}

		Damage();

		mSprite->stopAllActions();
		mSprite->runAction(actionOfPlayer[ATTACK]);
		currentAction = ATTACK;
	}
}

void Player::Slide()
{
	if (isOnLand && currentAction != SLIDE)
	{
		mSprite->stopAllActions();
		mSprite->runAction(actionOfPlayer[SLIDE]);
		currentAction = SLIDE;
		this->setIsMove(true);
	}

}

void Player::ThrowKunai()
{
	GameSetting* gameSetting = GameSetting::getInstance();
	int numberKunai = gameSetting->getNumberKunai();

	if (isOnLand && currentAction != THROWKUNAI)
	{

		if (GameSetting::getInstance()->isEnableSound())
		{
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/PhongPhiTieu.mp3", false);
		}
		GameSetting::getInstance()->setNumberKunai(numberKunai - 1);

		mSprite->stopAllActions();
		mSprite->runAction(actionOfPlayer[THROWKUNAI]);
		currentAction = THROWKUNAI;
		setKunai();
	}
	else if (!isOnLand)
	{
		if (currentAction == JUMPTHROW && mSprite->getNumberOfRunningActions() > 0)
		{

		}
		else
		{
			if (GameSetting::getInstance()->isEnableSound())
			{
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/PhongPhiTieu.mp3", false);
			}
			GameSetting::getInstance()->setNumberKunai(numberKunai - 1);

			mSprite->stopAllActions();
			mSprite->runAction(actionOfPlayer[JUMPTHROW]);
			currentAction = JUMPTHROW;
			setKunai();
		}
	}

}

int Player::getCurrentAction()
{
	return currentAction;
}

void Player::update()
{
	for (int i = 0; i < 10; i++)
	{
		if (kunais.at(i)->getIsDislay())
		{
			kunais.at(i)->update();
		}
	}

	if (powerSword->getIsDislay())
	{
		powerSword->update();
	}
}

void Player::setKunai()
{
	for (int i = 0; i < 10; i++)
	{
		if (!kunais.at(i)->getIsDislay())
		{
			kunais.at(i)->setIsDislay(true);

			if (directionRun == TURNLEFT)
			{
				kunais.at(i)->setPosition(this->getPositionX(), this->getPositionY() + 30);
				kunais.at(i)->setMoveByX(10);
				kunais.at(i)->setMoveByY(0);

			}
			else
			{
				kunais.at(i)->setPosition(this->getPositionX() + 40, this->getPositionY() + 30);
				kunais.at(i)->setMoveByX(-10);
				kunais.at(i)->setMoveByY(0);
			}

			break;
		}
	}
}

void Player::hasConclusion(int index)
{
	kunais.at(index)->setIsDislay(false);
}

void Player::Damage()
{
	scopeDamageBody->setEnabled(true);

	RotateTo* rotation;

	if (directionRun == TURNLEFT)
	{
		rotation = RotateTo::create(0.4f, -150);
	}
	else
	{
		rotation = RotateTo::create(0.4f, 150);
	}

	auto finishAttack = CallFunc::create([&]()
	{
		scopeDamageBody->setEnabled(false);
		scopeDamage->setRotation(0.0f);
	});

	auto sequence = Sequence::create(rotation, finishAttack, nullptr);

	scopeDamage->runAction(sequence);
}

void Player::unty()
{
	mSprite->stopAllActions();
	mSprite->runAction(actionOfPlayer[ATTACK]);
	if (GameSetting::getInstance()->isEnableSound() == true)
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/BigBang.mp3", false);
	}
	if (!powerSword->getIsDislay())
	{
		powerSword->setIsDislay(true);
		if (directionRun == TURNLEFT)
		{
			powerSword->setPosition(mSprite->getPositionX(), mSprite->getPositionY() + 30);
			powerSword->setFlippedX(true);
			powerSword->setMoveByX(10.0f);
			powerSword->setMoveByY(0.0f);

		}
		else
		{
			powerSword->setPosition(mSprite->getPositionX() + 40, mSprite->getPositionY() + 30);
			powerSword->setFlippedX(false);
			powerSword->setMoveByX(-10.0f);
			powerSword->setMoveByY(0.0f);
		}


	}

}

bool Player::getPreventLeft()
{
	return preventLeft;
}

void Player::setPreventLeft(bool preventLeft)
{
	this->preventLeft = preventLeft;
}

bool Player::getPreventRight()
{
	return preventRight;
}

void Player::setPreventRight(bool preventRight)
{
	this->preventRight = preventRight;
}

void Player::beingAttacked(int damage)
{
	if (GameSetting::getInstance()->isEnableSound())
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/BiDanhTrung.mp3", false);
	}

	auto bloodparticle = ParticleSystemQuad::create("particles/particle_blood.plist");
	bloodparticle->setPosition(Vec2(mSprite->getPosition().x, mSprite->getPosition().y + 40.0f));

	mSprite->getParent()->addChild(bloodparticle, 20);

	int realDamage = 0;

	realDamage = damage * ((float)100 / (100 + armor));

	this->setCurrentBlood(currentBlood - realDamage);
}

void Player::upLevel()
{
	GameSetting* gameSetting = GameSetting::getInstance();

	totalBlood = totalBlood + 20 + (gameSetting->getPlayerLevel() - 1)*5;
	currentBlood = currentBlood + 20 + (gameSetting->getPlayerLevel() - 1) * 5;
	damage = damage + 2 + (gameSetting->getPlayerLevel() - 1) * 2;
	armor = armor + 5 + (gameSetting->getPlayerLevel() - 1) * 3;

	gameSetting->setBloodPlayer(totalBlood);
	gameSetting->setDamePlayer(damage);
	gameSetting->setArmor(armor);
	gameSetting->setEXP(0);
	gameSetting->setPlayerLevel(gameSetting->getPlayerLevel() + 1);

	gameSetting->saveData();
}

void Player::setIsMove(bool isMove)
{
	this->isMove = isMove;
}

bool Player::getIsMove()
{
	return isMove;
}

void Player::setIsOnland(bool isOnland)
{
	this->isOnLand = isOnland;
}

bool Player::getIsOnland()
{
	return isOnLand;
}