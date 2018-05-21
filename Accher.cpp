#include "Model\Accher.h"
#include "Model\GameSetting.h"

#include "Define.h"

USING_NS_CC;

Accher::Accher(cocos2d::TMXTiledMap* layer)
{
	GameSetting* gameSetting = GameSetting::getInstance();
	totalBlood = 30.0f + (gameSetting->getLevel() - 1)*5.0f;
	currentBlood = 30.0f + (gameSetting->getLevel() - 1)*5.0f;
	damage = 5.0f + (gameSetting->getLevel() - 1)*3.0f;
	isAlive = true;
	isKeepKey = false;

	arrow = new Arrow(layer);

	mSprite = Sprite::createWithSpriteFrameName("actack_00.png");
	mSprite->setPosition(Vec2(0.0f, 0.0f));
	mSprite->setAnchorPoint(Vec2(0.5f, 0.0f));
	layer->addChild(mSprite,1);

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

	mPhysicsBody = PhysicsBody::createBox(mSprite->getContentSize(), PhysicsMaterial(1.0f, 0.0f, 1.0f));
	mPhysicsBody->setCollisionBitmask(ACCHER_BISMARK);
	mPhysicsBody->setContactTestBitmask(true);
	//mPhysicsBody->setCategoryBitmask(DYNAMIC_CATEGORY);
	mPhysicsBody->setMass(300.0f);
	mPhysicsBody->setRotationEnable(false);
	
	mSprite->setPhysicsBody(mPhysicsBody);

	this->setIsDislay(false);

	//create animate
	actionOfMonser[SHOOT] = Repeat::create(getAnimate("actack_0%d.png", 5, 0.2f), 1);

	CC_SAFE_RETAIN(actionOfMonser[SHOOT]);
}
Accher::~Accher()
{
	actionOfMonser[SHOOT]->autorelease();
}

bool Accher::Shooting(float xTarget, float yTarget)
{

	
	if (mSprite->getPosition().x - xTarget < 40 && mSprite->getPosition().x - xTarget > -40)
	{
		return false;
	}

	mSprite->runAction(actionOfMonser[SHOOT]);

	/*if (GameSetting::getInstance()->isEnableSound())
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/BanCung.mp3", false);
	}
*/
	if (xTarget < mSprite->getPosition().x)
	{
		this->turnLeft();

	}
	else
	{
		this->turnRight();
	}


	if (!arrow->getIsDislay())
	{		
		if (directionRun == TURNLEFT)
		{
			float xArrow = mSprite->getPosition().x - arrow->getContentSize().width - 5;
			float yArrow = mSprite->getPosition().y + mSprite->getContentSize().height*0.37f;
			float deltaX = xArrow - xTarget;
			float deltaY = yArrow - yTarget;
			float distance = sqrt((deltaX*deltaX) + (deltaY*deltaY));

			float angle = (asin(deltaY / distance)*-180) / 3.14;

			arrow->setPosition(xArrow, yArrow);
			arrow->setIsDislay(true);		
				
			arrow->setFlippedX(false);
			arrow->setRowtation(angle);

			arrow->setMoveByX(deltaX / distance * 5);
			arrow->setMoveByY(deltaY / distance * 5);
			
		}
		else
		{
			float xArrow = mSprite->getPosition().x + mSprite->getContentSize().width*1.0f + 2;
			float yArrow = mSprite->getPosition().y + mSprite->getContentSize().height*0.37f;
			float deltaX = xArrow - xTarget;
			float deltaY = yArrow - yTarget;
			float distance = sqrt((deltaX*deltaX) + (deltaY*deltaY));

			float angle = (asin(deltaY / distance)*-180) / 3.14;

			//log("angle: %f", angle);
			//log("xTarget: %f, yTarget: %f", xTarget,yTarget);

			arrow->setPosition(xArrow, yArrow);
			arrow->setIsDislay(true);
				
			arrow->setFlippedX(true);
			arrow->setRowtation(-angle);

			arrow->setMoveByX(deltaX / distance * 5);
			arrow->setMoveByY(deltaY / distance * 5);
		
			//log("moveByX: %f, moveByY: %f", deltaX / distance * 5, deltaY / distance * 5);
			//log("%f : %f", xArrow, yArrow);
		}
			
	}

	return true;

}

void Accher::update()
{
	if (arrow->getIsDislay())
	{
		arrow->update();
	}
}


void Accher::hasCollision()
{
	arrow->setIsDislay(false);
}

void Accher::setIndex(int index)
{
	mPhysicsBody->setGroup(index);
	arrow->setIndexGroup(index);
}