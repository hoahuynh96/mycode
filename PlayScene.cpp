#include "GUI\PlayScene.h"
#include "Model\Player.h"
#include "GUI\WaitScene.h"
#include "ui\CocosGUI.h"
#include "GUI\GameOverScene.h"
#include "Model\GameSetting.h"
#include "GUI\MonsterSecen.h"

#include "Define.h"


USING_NS_CC;

extern int gCount;

Scene* PlayScene::createScene()
{
	auto scene = Scene::createWithPhysics();
	//scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	scene->getPhysicsWorld()->setGravity(Vec2(0.0f, -820.0f));

	auto layer = PlayScene::create();
	scene->addChild(layer);
	return scene;
}

bool PlayScene::init()
{
	if (!Layer::init())
	{
		return false;
	}


	gCount = 0;

	visibleSize = Director::getInstance()->getVisibleSize();
	gameSetting = GameSetting::getInstance();

	if (gameSetting->getLevel() == 1)
	{
		physicsTileMap = TMXTiledMap::create("titlemaps/level1.tmx");
		addChild(physicsTileMap, 1);
	}
	else
	{
		physicsTileMap = TMXTiledMap::create("titlemaps/level2.tmx");
		addChild(physicsTileMap, 1);
	}
	auto objectLayer = physicsTileMap->getObjectGroup("object");

	auto objects = objectLayer->getObject("Player");
	int x = objects.at("x").asInt();
	int y = objects.at("y").asInt();

	mainPlayer = new Player(this);
	mainPlayer->setAnchorPoint(Vec2(0.5f, 0.0f));
	mainPlayer->setPosition(Vec2((float)x, (float)y));

	// create iconmoney animation
	auto spriteFrameCache = SpriteFrameCache::getInstance();
	Vector<SpriteFrame*> listImage;
	char str[100];
	for (int i = 0; i < 7; i++)
	{
		sprintf(str, "moneysprite_0%d.png", i);
		listImage.pushBack(spriteFrameCache->getSpriteFrameByName(str));
	}

	auto animation = Animation::createWithSpriteFrames(listImage, 0.1f);

	moneyAnimate = Animate::create(animation);
	CC_SAFE_RETAIN(moneyAnimate);

	if (gameSetting->isEnableMusic())
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
		CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("sounds/BGmusic.mp3");
	}

	createUi();

	createButton();

	createEnemy();

	createPhysic();

	createItem();

	auto touchEvent = EventListenerTouchOneByOne::create();

	touchEvent->onTouchBegan = CC_CALLBACK_2(PlayScene::onTouchBegan, this);
	touchEvent->onTouchMoved = CC_CALLBACK_2(PlayScene::onTouchMoved, this);
	touchEvent->onTouchEnded = CC_CALLBACK_2(PlayScene::onTouchEnded, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchEvent, this);

	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(PlayScene::onContactBegin, this);
	contactListener->onContactSeparate = CC_CALLBACK_1(PlayScene::onContactSeparate, this);
	contactListener->onContactPreSolve = CC_CALLBACK_1(PlayScene::onContactPreSolve, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

	auto keyListener = EventListenerKeyboard::create();
	keyListener->onKeyPressed = [&](EventKeyboard::KeyCode keycode, Event* event)
	{
		if (mainPlayer->getCurrentAction() != SLIDE)
		{
			switch (keycode)
			{
			case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
				moveLeftPress->setVisible(true);

				if (mainPlayer->getIsOnland())
				{
					mainPlayer->turnLeft();
				}

				if (mainPlayer->getDirection() == TURNLEFT)
				{
					mainPlayer->setIsMove(true);
					mainPlayer->setPreventRight(false);
				}
				break;
			case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
				moveRightPress->setVisible(true);

				if (mainPlayer->getIsOnland())
				{
					mainPlayer->turnRight();
				}

				if (mainPlayer->getDirection() == TURNRIGHT)
				{
					mainPlayer->setIsMove(true);
					mainPlayer->setPreventLeft(false);
				}
				break;
			default:
				break;
			}
			

			
		}
	};

	keyListener->onKeyReleased = [&](EventKeyboard::KeyCode keycode, Event* event)
	{

		if (mainPlayer->getCurrentAction() == SLIDE && mainPlayer->getCurrentAction() > 0)
		{

		}
		else
		{
			moveLeftPress->setVisible(false);
			moveRightPress->setVisible(false);

			if (mainPlayer->getCurrentAction() != JUMP)
			{
				mainPlayer->Idle();
			}

			mainPlayer->setIsMove(false);
		}
	};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyListener, this);

	scheduleUpdate();

	return true;
}

void PlayScene::update(float dt)
{
	if (mainPlayer->getCurrentBlood() <= 0 || mainPlayer->getPositionY() <= 0)
	{
		auto gameOverScene = GameOverScene::createScene();
		Director::getInstance()->replaceScene(gameOverScene);
	}
	
	setCenterView();

	mainPlayer->update();

	updateEnemy();

	gCount++;

	if (gCount % 100 == 0)
	{

		for (int i = 0; i < acchers.size(); i++)
		{
			if (acchers.at(i)->getIsDislay())
			{

				acchers.at(i)->Shooting(mainPlayer->getPositionX() - physicsTileMap->getPositionX(), mainPlayer->getPositionY() + 30);
			}
		}
	}

	for (int i = 0; i < acchers.size(); i++)
	{
		acchers.at(i)->update();
	}

	if (!mainPlayer->getIsMove() && mainPlayer->getNumberOfRunningActions() <= 0)
	{
		mainPlayer->Idle();
	}
	else if (mainPlayer->getIsMove() && mainPlayer->getNumberOfRunningActions() <= 0 && mainPlayer->getCurrentAction() == SLIDE)
	{
		mainPlayer->setIsMove(false);
	}
	else if (mainPlayer->getIsMove() && mainPlayer->getNumberOfRunningActions() <= 0 || mainPlayer->getIsMove() && mainPlayer->getCurrentAction() == IDLE)
	{
		mainPlayer->Run();
	}
}

void PlayScene::moveLeft()
{
	float x = 0;

	//neu vi tri cua nhan vat chinh con lon hon 0.2 man hinh, hoac lon hon 0 va map da den diem dau tien thi cho phep di chuyen
	if (mainPlayer->getPositionX() > visibleSize.width*0.2 || (mainPlayer->getPositionX()>25.0f && physicsTileMap->getPositionX() >= 0))
	{
		x = mainPlayer->getPositionX();
		mainPlayer->setPositionX(x - 4);
	}
	else
	{
		if (physicsTileMap->getPositionX() < 0)
		{
			physicsTileMap->setPositionX(physicsTileMap->getPositionX() + 4);
			backGround->scrollLeft();
		}
	}
}

void PlayScene::moveRight()
{
	float x;

	if (mainPlayer->getPositionX() < visibleSize.width / 2 || (mainPlayer->getPositionX() < visibleSize.width - 20.0f && physicsTileMap->getPositionX() + physicsTileMap->getContentSize().width <= visibleSize.width))
	{
		float x = mainPlayer->getPositionX();
		mainPlayer->setPositionX(x + 4);
	}
	else
	{
		if (physicsTileMap->getPositionX() + physicsTileMap->getContentSize().width > visibleSize.width)
		{
			physicsTileMap->setPositionX(physicsTileMap->getPositionX() - 4);
			backGround->scrollRight();
		}
	}
}

bool PlayScene::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
	if (mainPlayer->getCurrentAction() != SLIDE)
	{
		if (moveLeftNormal->getBoundingBox().containsPoint(touch->getLocation()))
		{
			moveLeftPress->setVisible(true);

			if (mainPlayer->getIsOnland())
			{
				mainPlayer->turnLeft();
			}

			if (mainPlayer->getDirection() == TURNLEFT)
			{
				mainPlayer->setIsMove(true);
				mainPlayer->setPreventRight(false);
			}
		}

		if (moveRightNormal->getBoundingBox().containsPoint(touch->getLocation()))
		{
			moveRightPress->setVisible(true);

			if (mainPlayer->getIsOnland())
			{
				mainPlayer->turnRight();
			}

			if (mainPlayer->getDirection() == TURNRIGHT)
			{
				mainPlayer->setIsMove(true);
				mainPlayer->setPreventLeft(false);
			}
		}
	}

	return true;
}

void PlayScene::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event)
{

	if (mainPlayer->getCurrentAction() == SLIDE && mainPlayer->getCurrentAction() > 0)
	{

	}
	else
	{
		if (moveLeftNormal->getBoundingBox().containsPoint(touch->getLocation()))
		{
			moveLeftPress->setVisible(true);
			if (mainPlayer->getIsOnland())
			{
				mainPlayer->turnLeft();
			}

			if (mainPlayer->getDirection() == TURNLEFT)
			{
				mainPlayer->setIsMove(true);
				mainPlayer->setPreventRight(false);
			}
		}
		else if (moveRightNormal->getBoundingBox().containsPoint(touch->getLocation()))
		{
			moveRightPress->setVisible(true);

			if (mainPlayer->getIsOnland())
			{
				mainPlayer->turnRight();
			}
			
			if (mainPlayer->getDirection() == TURNRIGHT)
			{
				mainPlayer->setIsMove(true);
				mainPlayer->setPreventLeft(false);
			}

		}
		else
		{
			moveLeftPress->setVisible(false);
			moveRightPress->setVisible(false);
			mainPlayer->setIsMove(false);
		}
	}
}

void PlayScene::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event)
{

	if (mainPlayer->getCurrentAction() == SLIDE && mainPlayer->getCurrentAction() > 0)
	{

	}
	else
	{
		moveLeftPress->setVisible(false);
		moveRightPress->setVisible(false);

		if (mainPlayer->getCurrentAction() != JUMP)
		{
			mainPlayer->Idle();
		}

		mainPlayer->setIsMove(false);
	}

}

void PlayScene::setCenterView()
{
	// neu nhan vat dang di chuyen thi thuc hien
	if (mainPlayer->getIsMove())
	{
		if (mainPlayer->getDirection() == TURNLEFT && !mainPlayer->getPreventLeft())
		{
			moveLeft();
		}
		else if (mainPlayer->getDirection() == TURNRIGHT && !mainPlayer->getPreventRight())
		{
			moveRight();
		}

	}
}

bool PlayScene::onContactBegin(cocos2d::PhysicsContact& contact)
{

	//Neu kieu tra la false thi di xuyen qua vat the

	auto shapeA = contact.getShapeA()->getBody();
	auto shapeB = contact.getShapeB()->getBody();

	//Xet monter va cham monter
	if (shapeA->getCollisionBitmask() == ASSASSIN_BISMARK || shapeA->getCollisionBitmask() == ACCHER_BISMARK || shapeA->getCollisionBitmask() == MERCENARY_BISKMARK)
	{
		if (shapeB->getCollisionBitmask() == ASSASSIN_BISMARK || shapeB->getCollisionBitmask() == ACCHER_BISMARK || shapeB->getCollisionBitmask() == MERCENARY_BISKMARK)
		{
			return false;
		}
	}

	//neu Player va cham monter thi khong xet su kien

	if (shapeA->getCollisionBitmask() == PLAYER_BISMARK && shapeB->getCollisionBitmask() == ASSASSIN_BISMARK || shapeA->getCollisionBitmask() == ASSASSIN_BISMARK && shapeB->getCollisionBitmask() == PLAYER_BISMARK)
	{
		return false;
	}

	if (shapeA->getCollisionBitmask() == PLAYER_BISMARK && shapeB->getCollisionBitmask() == ACCHER_BISMARK || shapeA->getCollisionBitmask() == ACCHER_BISMARK && shapeB->getCollisionBitmask() == PLAYER_BISMARK)
	{
		return false;
	}

	if (shapeA->getCollisionBitmask() == PLAYER_BISMARK && shapeB->getCollisionBitmask() == MERCENARY_BISKMARK || shapeA->getCollisionBitmask() == MERCENARY_BISKMARK && shapeB->getCollisionBitmask() == PLAYER_BISMARK)
	{
		return false;
	}

	//Neu player va cham voi nuoc thi chet


	if (shapeA->getCollisionBitmask() == PLAYER_BISMARK && shapeB->getCollisionBitmask() == WATER_BISMARK || shapeA->getCollisionBitmask() == WATER_BISMARK && shapeB->getCollisionBitmask() == PLAYER_BISMARK)
	{
		auto gameOverScene = GameOverScene::createScene();
		Director::getInstance()->replaceScene(gameOverScene);
	}

	// Player collect coins

	if (shapeA->getCollisionBitmask() == COINS_BISMARK)
	{
		if (shapeB->getCollisionBitmask() == PLAYER_BISMARK)
		{
			playerCollectItem(shapeA->getGroup());
		}
		
	}
	else if (shapeB->getCollisionBitmask() == COINS_BISMARK)
	{
		if (shapeA->getCollisionBitmask() == PLAYER_BISMARK)
		{
			playerCollectItem(shapeB->getGroup());
		}
		
	}

	//Xet player o tren mat dat

	if (shapeA->getCollisionBitmask() == PLAYER_BISMARK && shapeB->getCollisionBitmask() == ROUND_BISMARK || shapeA->getCollisionBitmask() == PLAYER_BISMARK && shapeB->getCollisionBitmask() == HARD_ROUND_BISMARK)
	{
		if (shapeA->getPosition().y > shapeB->getPosition().y + 30.0f)
		{
			mainPlayer->setIsOnland(true);

		}
	}
	else if (shapeA->getCollisionBitmask() == ROUND_BISMARK && shapeB->getCollisionBitmask() == PLAYER_BISMARK || shapeA->getCollisionBitmask() == HARD_ROUND_BISMARK && shapeB->getCollisionBitmask() == PLAYER_BISMARK)
	{
		if (shapeA->getPosition().y + 30.0f < shapeB->getPosition().y)
		{
			mainPlayer->setIsOnland(true);
		}
	}

	// Neu playler cham thung go thi cho phep nhay tiep
	if (shapeA->getCollisionBitmask() == PLAYER_BISMARK && shapeB->getCollisionBitmask() == WOOD_BISMARK)
	{
		if (shapeA->getPosition().y > shapeB->getPosition().y + 30.0f)
		{
			mainPlayer->setIsOnland(true);

		}
	}
	else if (shapeA->getCollisionBitmask() == WOOD_BISMARK && shapeB->getCollisionBitmask() == PLAYER_BISMARK)
	{
		if (shapeA->getPosition().y + 30.0f < shapeB->getPosition().y)
		{
			mainPlayer->setIsOnland(true);
		}
	}


	//Player va cham voi bom
	if (shapeA->getCollisionBitmask() == PLAYER_BISMARK && shapeB->getCollisionBitmask() == BOMP_BISMARK)
	{
		auto explode = ParticleSystemQuad::create("particles/particle_texture.plist");
		explode->setPosition(shapeB->getPosition());

		addChild(explode);
		physicsTileMap->removeChild(shapeB->getNode());

		mainPlayer->beingAttacked(10);
	}
	else if (shapeA->getCollisionBitmask() == BOMP_BISMARK && shapeB->getCollisionBitmask() == PLAYER_BISMARK)
	{

		auto explode = ParticleSystemQuad::create("particles/particle_texture.plist");
		explode->setPosition(shapeA->getPosition());

		addChild(explode);

		physicsTileMap->removeChild(shapeA->getNode());
		mainPlayer->beingAttacked(10);
	}

	//Player nhat duoc ruong vang
	if (shapeA->getCollisionBitmask() == PLAYER_BISMARK && shapeB->getCollisionBitmask() == GOOD_BISMARK)
	{
		physicsTileMap->removeChild(shapeB->getNode());

		updateMoney(400);
	}
	else if (shapeA->getCollisionBitmask() == GOOD_BISMARK && shapeB->getCollisionBitmask() == PLAYER_BISMARK)
	{
		physicsTileMap->removeChild(shapeA->getNode());

		updateMoney(400);
	}

	//Xet mui ten cham vao cac vat the vat ly

	if (shapeA->getCollisionBitmask() == ARROW_BISMARK)
	{
		acchers.at(shapeA->getGroup())->hasCollision();

		if (shapeB->getCollisionBitmask() == PLAYER_BISMARK)
		{
			mainPlayer->beingAttacked(acchers.at(0)->getDamage());

			updatePowerBar(acchers.at(0)->getDamage());
	
		}
	}

	if (shapeB->getCollisionBitmask() == ARROW_BISMARK)
	{
		acchers.at(shapeB->getGroup())->hasCollision();
		if (shapeA->getCollisionBitmask() == PLAYER_BISMARK)
		{

			mainPlayer->beingAttacked(acchers.at(0)->getDamage());

			updatePowerBar(acchers.at(0)->getDamage());
		}
	}

	//Xet phy tieu va cham vao vat ly

	if (shapeA->getCollisionBitmask() == KUNAI_BISMARK && shapeB->getCollisionBitmask() != PLAYER_BISMARK)
	{
		mainPlayer->hasConclusion(shapeA->getGroup());
		
		if (shapeB->getCollisionBitmask() == ACCHER_BISMARK)
		{
			acchers.at(shapeB->getGroup())->setCurrentBlood(acchers.at(shapeB->getGroup())->getCurrentBlood() - mainPlayer->getDamage()*1.2f);
		}
		else if (shapeB->getCollisionBitmask() == ASSASSIN_BISMARK)
		{
			assassins.at(shapeB->getGroup())->setCurrentBlood(assassins.at(shapeB->getGroup())->getCurrentBlood() - mainPlayer->getDamage()*1.2f);

		}
		else if (shapeB->getCollisionBitmask() == MERCENARY_BISKMARK)
		{
			mercenarys.at(shapeB->getGroup())->setCurrentBlood(mercenarys.at(shapeB->getGroup())->getCurrentBlood() - mainPlayer->getDamage()*1.2f);
		}
		else if (shapeB->getCollisionBitmask() == KNIGHT_BISMARK)
		{
			knights.at(shapeB->getGroup())->setCurrentBlood(knights.at(shapeB->getGroup())->getCurrentBlood() - mainPlayer->getDamage()*1.2f);
		}

	}

	if (shapeB->getCollisionBitmask() == KUNAI_BISMARK && shapeA->getCollisionBitmask() != PLAYER_BISMARK)
	{
		mainPlayer->hasConclusion(shapeB->getGroup());

		if (shapeA->getCollisionBitmask() == ACCHER_BISMARK)
		{
			acchers.at(shapeA->getGroup())->setCurrentBlood(acchers.at(shapeA->getGroup())->getCurrentBlood() - mainPlayer->getDamage()*1.2f);
		}
		else if (shapeA->getCollisionBitmask() == ASSASSIN_BISMARK)
		{
			assassins.at(shapeA->getGroup())->setCurrentBlood(assassins.at(shapeA->getGroup())->getCurrentBlood() - mainPlayer->getDamage()*1.2f);
		}
		else if (shapeA->getCollisionBitmask() == MERCENARY_BISKMARK)
		{
			mercenarys.at(shapeA->getGroup())->setCurrentBlood(mercenarys.at(shapeA->getGroup())->getCurrentBlood() - mainPlayer->getDamage()*1.2f);
		}
		else if (shapeA->getCollisionBitmask() == KNIGHT_BISMARK)
		{
			knights.at(shapeA->getGroup())->setCurrentBlood(knights.at(shapeA->getGroup())->getCurrentBlood() - mainPlayer->getDamage()*1.2f);
		}
	}

	//Player chem trung monters
	if (shapeA->getCollisionBitmask() == PLAYER_DAMAGE_BISMARK)
	{
		if (shapeB->getCollisionBitmask() == ASSASSIN_BISMARK)
		{
			assassins.at(shapeB->getGroup())->setCurrentBlood(assassins.at(shapeB->getGroup())->getCurrentBlood() - mainPlayer->getDamage());
			shapeA->setEnabled(false);		
		}
		else if (shapeB->getCollisionBitmask() == ACCHER_BISMARK)
		{
			acchers.at(shapeB->getGroup())->setCurrentBlood(acchers.at(shapeB->getGroup())->getCurrentBlood() - mainPlayer->getDamage());
			shapeA->setEnabled(false);
		}
		else if (shapeB->getCollisionBitmask() == MERCENARY_BISKMARK)
		{
			mercenarys.at(shapeB->getGroup())->setCurrentBlood(mercenarys.at(shapeB->getGroup())->getCurrentBlood() - mainPlayer->getDamage());
			shapeA->setEnabled(false);
		}
		else if (shapeB->getCollisionBitmask() == KNIGHT_BISMARK)
		{
			knights.at(shapeB->getGroup())->setCurrentBlood(knights.at(shapeB->getGroup())->getCurrentBlood() - mainPlayer->getDamage());
			shapeA->setEnabled(false);
		}
		else
		{
			return false;
		}
	}

	if (shapeB->getCollisionBitmask() == PLAYER_DAMAGE_BISMARK)
	{
		if (shapeA->getCollisionBitmask() == ASSASSIN_BISMARK)
		{
			assassins.at(shapeA->getGroup())->setCurrentBlood(assassins.at(shapeA->getGroup())->getCurrentBlood() - mainPlayer->getDamage());
			shapeB->setEnabled(false);
		}
		else if (shapeB->getCollisionBitmask() == ACCHER_BISMARK)
		{
			acchers.at(shapeA->getGroup())->setCurrentBlood(acchers.at(shapeA->getGroup())->getCurrentBlood() - mainPlayer->getDamage());
			shapeB->setEnabled(false);
		}
		else if (shapeA->getCollisionBitmask() == MERCENARY_BISKMARK)
		{
			mercenarys.at(shapeA->getGroup())->setCurrentBlood(mercenarys.at(shapeA->getGroup())->getCurrentBlood() - mainPlayer->getDamage());
			shapeB->setEnabled(false);
		}
		else if (shapeA->getCollisionBitmask() == KNIGHT_BISMARK)
		{
			knights.at(shapeA->getGroup())->setCurrentBlood(knights.at(shapeA->getGroup())->getCurrentBlood() - mainPlayer->getDamage());
			shapeB->setEnabled(false);
		}
		else
		{
			return false;
		}
	}

	//monters chem trung player
	if (shapeA->getCollisionBitmask() == MONTER_DAMAGE_BISMARK)
	{
		if (shapeB->getCollisionBitmask() == PLAYER_BISMARK)
		{
			mainPlayer->beingAttacked(assassins.at(0)->getDamage());

			updatePowerBar(assassins.at(0)->getDamage());

			shapeA->setEnabled(false);
		}
		else
		{
			return false;
		}
	}

	if (shapeB->getCollisionBitmask() == MONTER_DAMAGE_BISMARK)
	{
		if (shapeA->getCollisionBitmask() == PLAYER_BISMARK)
		{
			mainPlayer->beingAttacked(assassins.at(0)->getDamage());

			updatePowerBar(assassins.at(0)->getDamage());

			shapeB->setEnabled(false);
		}
		else
		{
			return false;
		}
	}

	//Xet powerSword va cham vat ly
	if (shapeA->getCollisionBitmask() == POWER_BISMARK)
	{
		if (shapeB->getCollisionBitmask() == ACCHER_BISMARK)
		{
			acchers.at(shapeB->getGroup())->setCurrentBlood(acchers.at(shapeB->getGroup())->getCurrentBlood() - mainPlayer->getDamage() * 3);
			return false;
		}
		else if (shapeB->getCollisionBitmask() == ASSASSIN_BISMARK)
		{
			assassins.at(shapeB->getGroup())->setCurrentBlood(assassins.at(shapeB->getGroup())->getCurrentBlood() - mainPlayer->getDamage() * 3);
			return false;
		}
		else if (shapeB->getCollisionBitmask() == MERCENARY_BISKMARK)
		{
			mercenarys.at(shapeB->getGroup())->setCurrentBlood(mercenarys.at(shapeB->getGroup())->getCurrentBlood() - mainPlayer->getDamage() * 3);
			return false;
		}
		else if (shapeB->getCollisionBitmask() == KNIGHT_BISMARK)
		{
			knights.at(shapeB->getGroup())->setCurrentBlood(knights.at(shapeB->getGroup())->getCurrentBlood() - mainPlayer->getDamage());
			shapeA->setEnabled(false);
		}
		else
		{
			return false;
		}
	}
	else if (shapeB->getCollisionBitmask() == POWER_BISMARK)
	{
		if (shapeA->getCollisionBitmask() == ACCHER_BISMARK)
		{
			acchers.at(shapeA->getGroup())->setCurrentBlood(acchers.at(shapeA->getGroup())->getCurrentBlood() - mainPlayer->getDamage() * 3);
			return false;
		}
		else if (shapeA->getCollisionBitmask() == ASSASSIN_BISMARK)
		{
			assassins.at(shapeA->getGroup())->setCurrentBlood(assassins.at(shapeA->getGroup())->getCurrentBlood() - mainPlayer->getDamage() * 3);
			return false;
		}
		else if (shapeA->getCollisionBitmask() == MERCENARY_BISKMARK)
		{
			mercenarys.at(shapeA->getGroup())->setCurrentBlood(mercenarys.at(shapeA->getGroup())->getCurrentBlood() - mainPlayer->getDamage() * 3);
			return false;
		}
		else if (shapeA->getCollisionBitmask() == KNIGHT_BISMARK)
		{
			knights.at(shapeA->getGroup())->setCurrentBlood(knights.at(shapeA->getGroup())->getCurrentBlood() - mainPlayer->getDamage());
			shapeB->setEnabled(false);
		}
		else
		{
			return false;
		}
	}


	//Xet Player contact hard physic
	if (shapeA->getCollisionBitmask() == HARD_ROUND_BISMARK && shapeB->getCollisionBitmask() == PLAYER_BISMARK)
	{
		if (mainPlayer->getPositionX() < shapeA->getPosition().x && mainPlayer->getPositionY() < shapeA->getNode()->getPosition().y + shapeA->getNode()->getContentSize().height - 8.0f)
		{
			mainPlayer->setPreventRight(true);
		}
		else if (mainPlayer->getPositionX() > shapeA->getPosition().x && mainPlayer->getPositionY() < shapeA->getNode()->getPosition().y + shapeA->getNode()->getContentSize().height - 8.0f)
		{
			mainPlayer->setPreventLeft(true);
		}

	}
	else if (shapeA->getCollisionBitmask() == PLAYER_BISMARK && shapeB->getCollisionBitmask() == HARD_ROUND_BISMARK)
	{
		if (mainPlayer->getPositionX() < shapeB->getPosition().x && mainPlayer->getPositionY() < shapeB->getNode()->getPosition().y + shapeB->getNode()->getContentSize().height - 2.0f)
		{
			mainPlayer->setPreventRight(true);
		}
		else if (mainPlayer->getPositionX() > shapeB->getPosition().x && mainPlayer->getPositionY() < shapeB->getNode()->getPosition().y + shapeB->getNode()->getContentSize().height - 2.0f)
		{
			mainPlayer->setPreventLeft(true);
		}

	}

	//Xet bullet Fire va cham Player va Ground
	if (shapeA->getCollisionBitmask() == BULLET_FIRE)
	{
		if (shapeB->getCollisionBitmask() == PLAYER_BISMARK)
		{
			mercenarys.at(shapeA->getGroup())->hasCollistion();

			mainPlayer->beingAttacked(mercenarys.at(shapeA->getGroup())->getDamage());

			updatePowerBar(mercenarys.at(shapeA->getGroup())->getDamage());
		}
		else if (shapeB->getCollisionBitmask() == MERCENARY_BISKMARK)
		{

		}
		else
		{
			mercenarys.at(shapeA->getGroup())->hasCollistion();
		}
	}
	else if (shapeB->getCollisionBitmask() == BULLET_FIRE)
	{
		if (shapeA->getCollisionBitmask() == PLAYER_BISMARK)
		{
			mercenarys.at(shapeB->getGroup())->hasCollistion();

			mainPlayer->beingAttacked(mercenarys.at(shapeB->getGroup())->getDamage());

			updatePowerBar(mercenarys.at(shapeB->getGroup())->getDamage());
		}
		else if (shapeB->getCollisionBitmask() == MERCENARY_BISKMARK)
		{

		}
		else
		{
			mercenarys.at(shapeB->getGroup())->hasCollistion();
		}
	}

	//Mercenary chem trung player
	if (shapeA->getCollisionBitmask() == MERCENARY_DAME_BISMARK)
	{
		if (shapeB->getCollisionBitmask() == PLAYER_BISMARK)
		{
			mainPlayer->beingAttacked(mercenarys.at(0)->getDamage());

			updatePowerBar(mercenarys.at(0)->getDamage());

			shapeA->setEnabled(false);
		}
		else
		{
			return false;
		}
	}

	if (shapeB->getCollisionBitmask() == MERCENARY_DAME_BISMARK)
	{
		if (shapeA->getCollisionBitmask() == PLAYER_BISMARK)
		{
			auto bloodparticle = ParticleSystemQuad::create("particles/particle_blood.plist");
			bloodparticle->setPosition(shapeA->getPosition());

			addChild(bloodparticle, 20);

			mainPlayer->beingAttacked(mercenarys.at(0)->getDamage());

			updatePowerBar(mercenarys.at(0)->getDamage());

			shapeB->setEnabled(false);
		}
		else
		{
			return false;
		}
	}

	// Player collect Key
	if (shapeA->getCollisionBitmask() == PLAYER_BISMARK&&shapeB->getCollisionBitmask() == KEY_BISMARK || shapeA->getCollisionBitmask() == KEY_BISMARK && shapeB->getCollisionBitmask() == PLAYER_BISMARK)
	{
		mainPlayer->setKeepKey(true);
		keyLabel->setString("1x");
		for (int i = 0; i < items.size(); i++)
		{
			if (items.at(i)->getTypeItem() == KEY_ITEM)
			{
				items.at(i)->setIsDislay(false);
				break;
			}
		}

		gate->getPhysicsBody()->setEnabled(true);
	}


	//Player mo cong
	if (shapeA->getCollisionBitmask() == PLAYER_BISMARK && shapeB->getCollisionBitmask() == GATE_BISMARK || shapeA->getCollisionBitmask() == GATE_BISMARK && shapeB->getCollisionBitmask() == PLAYER_BISMARK)
	{
		openGate();
	}

	return true;
}

void PlayScene::onContactSeparate(cocos2d::PhysicsContact& contact)
{
	auto shapeA = contact.getShapeA()->getBody();
	auto shapeB = contact.getShapeB()->getBody();

	//xet PlayerA roi khoi hard_Roud_Biskmark

	if (shapeA->getCollisionBitmask() == HARD_ROUND_BISMARK && shapeB->getCollisionBitmask() == PLAYER_BISMARK)
	{
		if (mainPlayer->getPositionX() < shapeA->getPosition().x)
		{
			mainPlayer->setPreventRight(false);
		}
		else if (mainPlayer->getPositionX() > shapeA->getPosition().x)
		{
			mainPlayer->setPreventLeft(false);
		}

	}
	else if (shapeA->getCollisionBitmask() == PLAYER_BISMARK && shapeB->getCollisionBitmask() == HARD_ROUND_BISMARK)
	{
		if (mainPlayer->getPositionX() < shapeB->getPosition().x)
		{
			mainPlayer->setPreventRight(false);
		}
		else if (mainPlayer->getPositionX() > shapeB->getPosition().x)
		{
			mainPlayer->setPreventLeft(false);
		}

	}

}

bool PlayScene::onContactPreSolve(cocos2d::PhysicsContact& contact)
{
	auto shapeA = contact.getShapeA()->getBody();
	auto shapeB = contact.getShapeB()->getBody();

	if (shapeA->getCollisionBitmask() == HARD_ROUND_BISMARK && shapeB->getCollisionBitmask() == PLAYER_BISMARK)
	{
		if (mainPlayer->getPositionX() < shapeA->getPosition().x && mainPlayer->getPositionY() < shapeA->getNode()->getPosition().y + shapeA->getNode()->getContentSize().height - 20.0f)
		{
			mainPlayer->setPreventRight(true);
		}
		else if (mainPlayer->getPositionX() > shapeA->getPosition().x && mainPlayer->getPositionY() < shapeA->getNode()->getPosition().y + shapeA->getNode()->getContentSize().height - 20.0f)
		{
			mainPlayer->setPreventLeft(true);
		}

	}
	else if (shapeA->getCollisionBitmask() == PLAYER_BISMARK && shapeB->getCollisionBitmask() == HARD_ROUND_BISMARK)
	{
		if (mainPlayer->getPositionX() < shapeB->getPosition().x && mainPlayer->getPositionY() < shapeB->getNode()->getPosition().y + shapeB->getNode()->getContentSize().height - 20.0f)
		{
			mainPlayer->setPreventRight(true);
		}
		else if (mainPlayer->getPositionX() > shapeB->getPosition().x && mainPlayer->getPositionY() < shapeB->getNode()->getPosition().y + shapeB->getNode()->getContentSize().height - 20.0f)
		{
			mainPlayer->setPreventLeft(true);
		}

	}


	return true;
}


void PlayScene::createEnemy()
{
	auto monterGroup = physicsTileMap->getObjectGroup("object");
	auto monters = monterGroup->getObjects();

	//Random position keep Key
	int numberObject = 0;
	for (int i = 0; i < monters.size(); i++)
	{
		int type = monters.at(i).asValueMap().at("type").asInt();
		if (type == 3 || type == 2 || type == 7 || type == 9)
		{
			numberObject++;
		}
	}

	int positionKey = random(0, numberObject-1);

	int indexEnamy = 0;
	int indexAccher = 0;
	int indexAssassin = 0;
	int indexMercenary = 0;
	int indexKnight = 0;

	for (int i = 0; i < monters.size(); i++)
	{
		float x = monters.at(i).asValueMap().at("x").asFloat();
		float y = monters.at(i).asValueMap().at("y").asFloat();

		if (monters.at(i).asValueMap().at("type").asInt() == 3)
		{

			float maxX = monters.at(i).asValueMap().at("maxX").asFloat();
			float minX = monters.at(i).asValueMap().at("minX").asFloat();

			Assassin* assassin = new Assassin(physicsTileMap);
			assassin->setPosition(Vec2((float)x, (float)y));		
			assassin->setIndex(indexAssassin++);
			assassin->setMaxPosition(maxX);
			assassin->setMinPosition(minX);

			if (indexEnamy == positionKey)
			{
				assassin->setKeepKey(true);
			}
			indexEnamy++;

			assassins.push_back(assassin);
		}
		else if (monters.at(i).asValueMap().at("type").asInt() == 2)
		{
			Accher* accher = new Accher(physicsTileMap);
			accher->setPosition(Vec2((float)x, (float)y));
			accher->setIndex(indexAccher++);


			if (indexEnamy == positionKey)
			{
				accher->setKeepKey(true);
			}
			indexEnamy++;

			acchers.push_back(accher);
		}
		else if (monters.at(i).asValueMap().at("type").asInt() == 7)
		{
	
			float maxX = monters.at(i).asValueMap().at("maxX").asFloat();
			float minX = monters.at(i).asValueMap().at("minX").asFloat();

			Mercenary* mercenary = new Mercenary(physicsTileMap);
			mercenary->setPosition(Vec2((float)x, (float)y));
			mercenary->setIndex(indexMercenary++);
			mercenary->setMinPosition(minX);
			mercenary->setMaxPosition(maxX);


			if (indexEnamy == positionKey)
			{
				mercenary->setKeepKey(true);
			}
			indexEnamy++;

			mercenarys.push_back(mercenary);
		}
		else if (monters.at(i).asValueMap().at("type").asInt() == 9)
		{

			float maxX = monters.at(i).asValueMap().at("maxX").asFloat();
			float minX = monters.at(i).asValueMap().at("minX").asFloat();

			Knight* knight = new Knight(physicsTileMap);
			knight->setPosition(Vec2((float)x, (float)y));
			knight->setIndex(indexKnight++);
			knight->setMinPosition(minX);
			knight->setMaxPosition(maxX);


			if (indexEnamy == positionKey)
			{
				knight->setKeepKey(true);
			}
			indexEnamy++;

			knights.push_back(knight);
		}
	}

}

void PlayScene::createUi()
{
	backGround = new Background(this, "basd.png");

	char str[20];
	sprintf(str, "%d", gameSetting->getMoney());

	money = Label::create();
	money->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - 20.0f));
	money->setSystemFontSize(28.0f);
	money->setColor(Color3B::RED);
	money->setString(str);
	this->addChild(money);

	auto iconMoney = Sprite::createWithSpriteFrameName("money.png");
	iconMoney->setPosition(Vec2(visibleSize.width / 2 - 60.0f, visibleSize.height - 20.0f));
	this->addChild(iconMoney);

	powerBar = ui::LoadingBar::create();
	powerBar->loadTexture("powerBar.png", ui::Widget::TextureResType::PLIST);
	powerBar->setPercent(0.0f);
	powerBar->setPosition(Vec2(99.0f, 408.0f));
	powerBar->setAnchorPoint(Vec2(0.0f, 0.0f));
	powerBar->setDirection(ui::LoadingBar::Direction::LEFT);
	addChild(powerBar, 11);

	expLabel = Label::createWithTTF("+ 1EXP", "fonts/Marker Felt.ttf", 24);
	expLabel->setPosition(Vec2(-100, -100));
	expLabel->setVisible(false);
	expLabel->setTextColor(Color4B::BLACK);
	addChild(expLabel, 10);


	sprintf(str, "Lv %d", gameSetting->getPlayerLevel());

	levelLabel = Label::create(str, "fonts/Vnaris.ttf", 30.0f);
	levelLabel->setPosition(Vec2(50.0f, visibleSize.height - 90.0f));
	levelLabel->setTextColor(Color4B::RED);

	addChild(levelLabel, 10);

	expBar = ui::LoadingBar::create();
	expBar->loadTexture("expBar.png", ui::Widget::TextureResType::PLIST);
	expBar->setPercent((float)gameSetting->getEXP() / (gameSetting->getPlayerLevel() * 50)*100.0f);
	expBar->setDirection(ui::LoadingBar::Direction::LEFT);
	expBar->setRotation(-90.0f);
	expBar->setAnchorPoint(Vec2(0.0f, 0.0f));
	expBar->setPosition(Vec2(84.0f, 408.0f));

	addChild(expBar, 11);

	keyLabel = Label::createWithSystemFont("0x", "fonts/Vnaris.ttf", 30.0f);
	keyLabel->setPosition(Vec2(visibleSize.width*0.6f, visibleSize.height - 20));
	keyLabel->setTextColor(Color4B::YELLOW);

	addChild(keyLabel, 11);

	auto keyimage = Sprite::createWithSpriteFrameName("key.png");
	keyimage->setPosition(Vec2(visibleSize.width*0.7f, visibleSize.height - 20));
	keyimage->setScale(0.5f);

	addChild(keyimage, 11);

}

void PlayScene::createButton()
{
	//Board menu
	auto boardMenu = Sprite::create("ui/board.png");
	boardMenu->setPosition(visibleSize / 2);
	boardMenu->setContentSize(Size(300.0f, 120.0f));
	boardMenu->setVisible(false);
	addChild(boardMenu, 10);

	//home button
	auto homeButton = ui::Button::create();
	homeButton->loadTextureNormal("homenormal.png", ui::Widget::TextureResType::PLIST);
	homeButton->loadTexturePressed("homepress.png", ui::Widget::TextureResType::PLIST);
	homeButton->setPosition(Vec2(boardMenu->getContentSize().width / 2 - 20, boardMenu->getContentSize().height / 2));
	homeButton->setAnchorPoint(Vec2(1.0f, 0.5f));
	homeButton->setScale(0.5f);
	boardMenu->addChild(homeButton);
	homeButton->addClickEventListener([](Ref* render){
		auto waitScene = WaitScene::createScene();
		Director::getInstance()->replaceScene(waitScene);
		Director::getInstance()->resume();
	});

	//replay button
	auto replayButton = ui::Button::create();
	replayButton->loadTextureNormal("replaynormal.png", ui::Widget::TextureResType::PLIST);
	replayButton->loadTexturePressed("replaypress.png", ui::Widget::TextureResType::PLIST);
	replayButton->setPosition(Vec2(boardMenu->getContentSize().width / 2 + 20, boardMenu->getContentSize().height / 2));
	replayButton->setAnchorPoint(Vec2(0.0f, 0.5f));
	replayButton->setScale(0.5f);
	boardMenu->addChild(replayButton);
	replayButton->addClickEventListener([=](Ref* render){
		boardMenu->setVisible(false);
		Director::getInstance()->resume();
	});

	//Pause button
	auto pauseButton = ui::Button::create();
	pauseButton->loadTextureNormal("pausenormal.png", ui::Widget::TextureResType::PLIST);
	pauseButton->loadTexturePressed("pausepress.png", ui::Widget::TextureResType::PLIST);
	pauseButton->setPosition(visibleSize);
	pauseButton->setAnchorPoint(Vec2(1.0f, 1.0f));
	addChild(pauseButton, 15);
	pauseButton->addClickEventListener([=](Ref* render){
		boardMenu->setVisible(true);
		Director::getInstance()->pause();
	});

	//turn left button
	moveLeftNormal = Sprite::createWithSpriteFrameName("turnleftnormal.png");
	moveLeftNormal->setPosition(Vec2(65.0f, 50.0f));
	addChild(moveLeftNormal, 5);

	moveLeftPress = Sprite::createWithSpriteFrameName("turnleftpress.png");
	moveLeftPress->setPosition(Vec2(65.0f, 50.0f));
	moveLeftPress->setVisible(false);
	addChild(moveLeftPress, 6);

	//turn left button
	moveRightNormal = Sprite::createWithSpriteFrameName("turnrightnormal.png");
	moveRightNormal->setPosition(Vec2(140.0f, 50.0f));
	addChild(moveRightNormal, 5);

	moveRightPress = Sprite::createWithSpriteFrameName("turnrightpress.png");
	moveRightPress->setPosition(Vec2(140.0f, 50.0f));
	moveRightPress->setVisible(false);
	addChild(moveRightPress, 6);

	//jump button
	auto jumpButton = ui::Button::create();
	jumpButton->loadTextureNormal("jumpnormal.png", ui::Widget::TextureResType::PLIST);
	jumpButton->loadTexturePressed("jumppress.png", ui::Widget::TextureResType::PLIST);
	jumpButton->setPosition(Vec2(visibleSize.width - jumpButton->getContentSize().width*2.6f, jumpButton->getContentSize().height*0.5f + 5.0f));
	addChild(jumpButton, 10);
	jumpButton->addClickEventListener([=](Ref* render)
	{
		mainPlayer->Jump();
	});

	//attack button
	auto attackButton = ui::Button::create();
	attackButton->loadTextureNormal("attacknormal.png", ui::Widget::TextureResType::PLIST);
	attackButton->loadTexturePressed("attackpress.png", ui::Widget::TextureResType::PLIST);
	attackButton->setPosition(Vec2(visibleSize.width - 25.0f - attackButton->getContentSize().width / 2, attackButton->getContentSize().height / 2 + 25.0f));
	attackButton->setScale(1.8f);
	addChild(attackButton, 10);
	attackButton->addClickEventListener([=](Ref* render)
	{
		mainPlayer->Attack();
	});

	//slide button
	auto slideButton = ui::Button::create();
	slideButton->loadTextureNormal("slidenormal.png", ui::Widget::TextureResType::PLIST);
	slideButton->loadTexturePressed("slidepress.png", ui::Widget::TextureResType::PLIST);
	slideButton->setPosition(Vec2(visibleSize.width - 5.0f - slideButton->getContentSize().width / 2, slideButton->getContentSize().height * 2.6f));
	addChild(slideButton, 10);
	slideButton->addClickEventListener([=](Ref* render)
	{
		mainPlayer->Slide();
	});

	//kunainormal button
	kunaiButton = ui::Button::create();
	kunaiButton->loadTextureNormal("kunainormal.png", ui::Widget::TextureResType::PLIST);
	kunaiButton->loadTexturePressed("kunaipress.png", ui::Widget::TextureResType::PLIST);
	kunaiButton->loadTextureDisabled("kunaidisable.png", ui::Widget::TextureResType::PLIST);
	kunaiButton->setPosition(Vec2(visibleSize.width - kunaiButton->getContentSize().width * 2.5f, kunaiButton->getContentSize().height * 1.7f));
	addChild(kunaiButton, 10);
	kunaiButton->addClickEventListener([=](Ref* render)
	{
		mainPlayer->ThrowKunai();
		updateKunai();
	});

	//kunai label
	char str[20];
	sprintf(str, "%d", gameSetting->getNumberKunai());

	numberKunaiLabel = Label::create();
	numberKunaiLabel->setPosition(kunaiButton->getContentSize() / 2);
	numberKunaiLabel->setString(str);
	numberKunaiLabel->setSystemFontSize(25.0f);
	numberKunaiLabel->setTextColor(Color4B::BLACK);
	kunaiButton->addChild(numberKunaiLabel);

	if (atoi(str) <= 0)
	{
		kunaiButton->setBright(false);
		kunaiButton->setEnabled(false);
	}

	//create spell blood button
	spellButton = ui::Button::create();
	spellButton->loadTextureNormal("bloodnormal.png", ui::Widget::TextureResType::PLIST);
	spellButton->loadTexturePressed("bloodpress.png", ui::Widget::TextureResType::PLIST);
	spellButton->loadTextureDisabled("blooddisable.png", ui::Widget::TextureResType::PLIST);
	spellButton->setPosition(Vec2(visibleSize.width - 10.0f - spellButton->getContentSize().width / 2, spellButton->getContentSize().height * 3.5f + 10.0f));
	spellButton->addClickEventListener([&](Ref* render)
	{
		updateBloodSpell();
	});
	addChild(spellButton, 10);

	//create spell blood label
	sprintf(str, "%d", gameSetting->getBloodSpell());

	bloodSpellLabel = Label::create();
	bloodSpellLabel->setPosition(Vec2(spellButton->getContentSize() / 2));
	bloodSpellLabel->setString(str);
	bloodSpellLabel->setTextColor(Color4B::BLACK);
	spellButton->addChild(bloodSpellLabel, 10);

	if (atoi(str) <= 0)
	{
		spellButton->setBright(false);
		spellButton->setEnabled(false);
	}

	//create unty button
	untyButton = ui::Button::create();
	untyButton->loadTextureNormal("untynormal.png", ui::Widget::TextureResType::PLIST);
	untyButton->loadTexturePressed("untypress.png", ui::Widget::TextureResType::PLIST);
	untyButton->loadTextureDisabled("untydisable.png", ui::Widget::TextureResType::PLIST);
	untyButton->setPosition(Vec2(visibleSize.width - untyButton->getContentSize().width*1.7f, untyButton->getContentSize().height*2.5f));
	untyButton->addClickEventListener([&](Ref* render)
	{
		mainPlayer->unty();
		powerBar->setPercent(0.0f);
		untyButton->setBright(false);
		untyButton->setEnabled(false);
	});
	addChild(untyButton, 10);

	untyButton->setBright(false);
	untyButton->setEnabled(false);
}

void PlayScene::createPhysic()
{
	//Tao physics cho Map vat ly
	auto layerPhysic = physicsTileMap->getLayer("physic");
	Size layerSize = layerPhysic->getLayerSize();


	for (int i = 0; i < layerSize.width; i++)
	{
		for (int j = 0; j < layerSize.height; j++)
		{
			auto titleSet = layerPhysic->getTileAt(Vec2(i, j));
			if (titleSet != NULL)
			{
				auto titleSetBody = PhysicsBody::createBox(titleSet->getContentSize(), PhysicsMaterial(0.0f, 0.0f, 1.0f));
				titleSetBody->setDynamic(false);
				titleSetBody->setMass(1000);
				titleSetBody->setCollisionBitmask(ROUND_BISMARK);
				titleSetBody->setContactTestBitmask(true);
				//titleSetBody->setCategoryBitmask(GROUND_CATEGORY);
				titleSet->setPhysicsBody(titleSetBody);
			}
		}
	}


	//Tao hard physics cho map
	auto layerHardPhysic = physicsTileMap->getLayer("hardphysic");
	Size hardPhysicsLayer = layerHardPhysic->getLayerSize();

	for (int i = 0; i < hardPhysicsLayer.width; i++)
	{
		for (int j = 0; j < hardPhysicsLayer.height; j++)
		{
			auto titleSet = layerHardPhysic->getTileAt(Vec2(i, j));
			if (titleSet != NULL)
			{
				auto titleSetBody = PhysicsBody::createBox(titleSet->getContentSize(), PhysicsMaterial(0.0f, 0.0f, 1.0f));
				titleSetBody->setDynamic(false);
				titleSetBody->setMass(1000);
				titleSetBody->setCollisionBitmask(HARD_ROUND_BISMARK);
				//titleSetBody->setCategoryBitmask(GROUND_CATEGORY);
				titleSetBody->setContactTestBitmask(true);
				titleSet->setPhysicsBody(titleSetBody);
			}
		}
	}


	//Tao bay cho map
	auto trapPhysic = physicsTileMap->getLayer("water");
	Size trapSize = trapPhysic->getLayerSize();

	for (int i = 0; i < trapSize.width; i++)
	{
		for (int j = 0; j < trapSize.height; j++)
		{
			auto tileSet = trapPhysic->getTileAt(Vec2(i, j));
			if (tileSet != NULL)
			{
				auto tileSetBody = PhysicsBody::createBox(tileSet->getContentSize(), PhysicsMaterial(1.0f, 0.0f, 1.0f));
				tileSetBody->setDynamic(false);
				tileSetBody->setMass(200);
				tileSetBody->setCollisionBitmask(WATER_BISMARK);
				tileSetBody->setContactTestBitmask(true);
				//tileSetBody->setCategoryBitmask(MONTER_CATEGORY);
				tileSet->setPhysicsBody(tileSetBody);
			}
		}
	}
}

void PlayScene::createItem()
{
	for (int i = 0; i < 8; i++)
	{
		Item* item = new Item(physicsTileMap, "moneysprite_06.png", COINS_ITEM);
		item->setGroupIndex(i);
		items.push_back(item);
	}

	Item *item = new Item(physicsTileMap, "key.png", KEY_ITEM);
	items.push_back(item);

	auto objectGroup = physicsTileMap->getObjectGroup("object");
	auto objects = objectGroup->getObjects();

	for (int i = 0; i < objects.size(); i++)
	{
		float x = objects.at(i).asValueMap().at("x").asFloat();
		float y = objects.at(i).asValueMap().at("y").asFloat();
		if (objects.at(i).asValueMap().at("type").asInt() == 4)
		{

			auto woodbox = Sprite::createWithSpriteFrameName("woodbox.png");
			woodbox->setPosition(Vec2((float)x, (float)y));
			woodbox->setAnchorPoint(Vec2(0.0f, 0.0f));
			physicsTileMap->addChild(woodbox, 3);

			auto woodBoxBody = PhysicsBody::createBox(woodbox->getContentSize(), PhysicsMaterial(10.0f, 0.0f, 0.2f));
			woodBoxBody->setCollisionBitmask(WOOD_BISMARK);
			woodBoxBody->setContactTestBitmask(true);
			woodBoxBody->setMass(100.0f);
			woodbox->setPhysicsBody(woodBoxBody);
		}
		else if (objects.at(i).asValueMap().at("type").asInt() == 5)
		{
			auto bompBox = Sprite::createWithSpriteFrameName("bompbox.png");
			bompBox->setPosition(Vec2((float)x, (float)y));
			bompBox->setAnchorPoint(Vec2(0.0f, 0.0f));
			physicsTileMap->addChild(bompBox, 3);

			auto bompBoxBody = PhysicsBody::createBox(bompBox->getContentSize(), PhysicsMaterial(1.0f, 0.0f, 1.0f));
			bompBoxBody->setCollisionBitmask(BOMP_BISMARK);
			bompBoxBody->setContactTestBitmask(true);
			//bompBoxBody->setCategoryBitmask(ITEM_CATEGORY);
			bompBox->setPhysicsBody(bompBoxBody);
		}
		else if (objects.at(i).asValueMap().at("type").asInt() == 6)
		{
			auto goodBox = Sprite::create("ui/ruongvang2.png");
			goodBox->setPosition(Vec2((float)x, (float)y));
			goodBox->setAnchorPoint(Vec2(0.0f, 0.0f));

			physicsTileMap->addChild(goodBox, 3);

			auto goodBoxBody = PhysicsBody::createBox(goodBox->getContentSize(), PhysicsMaterial(1.0f, 0.0f, 1.0f));
			goodBoxBody->setCollisionBitmask(GOOD_BISMARK);
			goodBoxBody->setContactTestBitmask(true);
			//goodBoxBody->setCategoryBitmask(ITEM_CATEGORY);
			goodBox->setPhysicsBody(goodBoxBody);
		}
		else if (objects.at(i).asValueMap().at("type").asInt() == 8)
		{
			gate = Sprite::createWithSpriteFrameName("lv1_gate0.png");
			gate->setAnchorPoint(Vec2(0.0f, 0.0f));
			gate->setScale(1.3f);
			gate->setPosition(Vec2(x, y));

			physicsTileMap->addChild(gate, 11);

			auto gateBody = PhysicsBody::createBox(gate->getContentSize());
			gateBody->setCollisionBitmask(GATE_BISMARK);
			gateBody->setContactTestBitmask(true);
			gateBody->setDynamic(false);
			gateBody->setMass(5.0f);
			gateBody->setEnabled(false);

			gate->setPhysicsBody(gateBody);
		}
	}


}

void PlayScene::updateEnemy()
{
	//Cap nhat trang thai va huong danh cho linh chem
	for (int i = 0; i < assassins.size(); i++)
	{
		if (assassins.at(i)->getCurrentBlood() <= 0 && assassins.at(i)->getIsAlive())
		{
			assassins.at(i)->setIsAlive(false);
			int type = assassins.at(i)->getKeepKey() ? KEY_ITEM : COINS_ITEM;

			showItem(assassins.at(i)->getPositionX(), assassins.at(i)->getPositionY() + 30.0f, type);
			playerCollectExp(1, assassins.at(i)->getPosition() + physicsTileMap->getPosition());
		}

		//Neu Linh con song moi tien hanh update
		if (assassins.at(i)->getIsAlive())
		{

			if (assassins.at(i)->getPositionX() + physicsTileMap->getPositionX() > -assassins.at(i)->getContentSize().width && assassins.at(i)->getPositionX() + physicsTileMap->getPositionX() < visibleSize.width + 20.0f)
			{
				assassins.at(i)->setIsDislay(true);

			}
			else
			{
				assassins.at(i)->setIsDislay(false);
			}

			//set huong tan cong cho quoai

			if (assassins.at(i)->getIsDislay())
			{
				float minPositionX = assassins.at(i)->getMinPosition() + physicsTileMap->getPositionX();
				float maxPositionX = assassins.at(i)->getMaxPosition() + physicsTileMap->getPositionX();

				float assasinPositionX = assassins.at(i)->getPositionX() + physicsTileMap->getPositionX();

				if (assasinPositionX - mainPlayer->getPositionX() < 60.0f && assasinPositionX - mainPlayer->getPositionX() > -55.0f && assassins.at(i)->getPositionY() - mainPlayer->getPositionY() < 80.0f && assassins.at(i)->getPositionY() - mainPlayer->getPositionY() > -80.0f)
				{
					if (assasinPositionX <= mainPlayer->getPositionX())
					{
						assassins.at(i)->turnRight();
					}
					else
					{
						assassins.at(i)->turnLeft();
					}
					assassins.at(i)->Attack();

				}
				else
				{
					if (mainPlayer->getPositionX() > minPositionX && mainPlayer->getPositionX() < maxPositionX && assassins.at(i)->getPositionY() - mainPlayer->getPositionY() < 80.0f && assassins.at(i)->getPositionY() - mainPlayer->getPositionY() > -80.0f)
					{
						assassins.at(i)->chase(mainPlayer->getPositionX() - physicsTileMap->getPositionX());
					}
					else
					{
						assassins.at(i)->autoWalk();
					}
				}
			}

		}
	}

	//Cap nhat trang thai cho knight
	for (int i = 0; i < knights.size(); i++)
	{
		if (knights.at(i)->getCurrentBlood() <= 0 && knights.at(i)->getIsAlive())
		{
			knights.at(i)->setIsAlive(false);
			int type = knights.at(i)->getKeepKey() ? KEY_ITEM : COINS_ITEM;

			showItem(knights.at(i)->getPositionX(), knights.at(i)->getPositionY() + 30.0f, type);
			playerCollectExp(1, knights.at(i)->getPosition() + physicsTileMap->getPosition());
		}

		//Neu Linh con song moi tien hanh update
		if (knights.at(i)->getIsAlive())
		{

			if (knights.at(i)->getPositionX() + physicsTileMap->getPositionX() > -knights.at(i)->getContentSize().width/2 && knights.at(i)->getPositionX() + physicsTileMap->getPositionX() < visibleSize.width + 20.0f)
			{
				knights.at(i)->setIsDislay(true);

			}
			else
			{
				knights.at(i)->setIsDislay(false);
			}

			//set huong tan cong cho quoai

			if (knights.at(i)->getIsDislay())
			{
				float minPositionX = knights.at(i)->getMinPosition() + physicsTileMap->getPositionX();
				float maxPositionX = knights.at(i)->getMaxPosition() + physicsTileMap->getPositionX();

				float knightPositionX = knights.at(i)->getPositionX() + physicsTileMap->getPositionX();

				if (knightPositionX - mainPlayer->getPositionX() < 60.0f && knightPositionX - mainPlayer->getPositionX() > -55.0f && knights.at(i)->getPositionY() - mainPlayer->getPositionY() < 80.0f && knights.at(i)->getPositionY() - mainPlayer->getPositionY() > -80.0f)
				{
					if (knightPositionX <= mainPlayer->getPositionX())
					{
						knights.at(i)->turnRight();
					}
					else
					{
						knights.at(i)->turnLeft();
					}
					knights.at(i)->Attack();
				}
				else
				{
					if (mainPlayer->getPositionX() > minPositionX && mainPlayer->getPositionX() < maxPositionX && knights.at(i)->getPositionY() - mainPlayer->getPositionY() < 80.0f && knights.at(i)->getPositionY() - mainPlayer->getPositionY() > -80.0f)
					{
						knights.at(i)->chase(mainPlayer->getPositionX() - physicsTileMap->getPositionX());
					}
					else
					{
						knights.at(i)->autoWalk();
					}
				}
			}

		}
	}

	//Cap nhat trang thai cho linh cung
	for (int i = 0; i < acchers.size(); i++)
	{
		if (acchers.at(i)->getCurrentBlood() <= 0 && acchers.at(i)->getIsAlive())
		{
			int type = acchers.at(i)->getKeepKey() ? KEY_ITEM : COINS_ITEM;

			acchers.at(i)->setIsAlive(false);
			showItem(acchers.at(i)->getPositionX(), acchers.at(i)->getPositionY() + 30.0f, type);
			playerCollectExp(1, acchers.at(i)->getPosition());
		}

		if (acchers.at(i)->getIsAlive())
		{
			if (acchers.at(i)->getPositionX() + physicsTileMap->getPositionX() > -acchers.at(i)->getContentSize().width/2 && acchers.at(i)->getPositionX() + physicsTileMap->getPositionX() < visibleSize.width + 20.0f)
			{
				acchers.at(i)->setIsDislay(true);
			}
			else
			{
				acchers.at(i)->setIsDislay(false);
			}
		}
	}

	//Cap nhat trang thai cho linh danh thue
	for (int i = 0; i < mercenarys.size(); i++)
	{
		mercenarys.at(i)->updateFire();
		if (mercenarys.at(i)->getCurrentBlood() <= 0 && mercenarys.at(i)->getIsAlive())
		{
			int type = mercenarys.at(i)->getKeepKey() ? KEY_ITEM : COINS_ITEM;

			mercenarys.at(i)->setIsAlive(false);
			showItem(mercenarys.at(i)->getPositionX(), mercenarys.at(i)->getPositionY() + 30.0f, type);
			playerCollectExp(1, mercenarys.at(i)->getPosition() + physicsTileMap->getPosition());
		}

		//Neu Linh con song moi tien hanh update
		if (mercenarys.at(i)->getIsAlive())
		{

			if (mercenarys.at(i)->getPositionX() + physicsTileMap->getPositionX() > -mercenarys.at(i)->getContentSize().width/2 && mercenarys.at(i)->getPositionX() + physicsTileMap->getPositionX() < visibleSize.width + 20.0f)
			{
				mercenarys.at(i)->setIsDislay(true);

			}
			else
			{
				mercenarys.at(i)->setIsDislay(false);
			}


			//Neu linh danh thue duoc hien thi tren man hinh thi cap nhat
			if (mercenarys.at(i)->getIsDislay())
			{
				mercenarys.at(i)->chase(mainPlayer->getPositionX() - physicsTileMap->getPositionX());
			}

		}
	}
}

void PlayScene::updateMoney(int money)
{
	gameSetting->setMoney(gameSetting->getMoney() + money);

	char moneyValue[20];
	sprintf(moneyValue, "%d", gameSetting->getMoney());

	this->money->setString(moneyValue);

	gameSetting->saveData();
	if (GameSetting::getInstance()->isEnableSound())
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/LuomTien.wav", false);
	}
}

void PlayScene::updateKunai()
{
	char numberKunai[20];
	sprintf(numberKunai, "%d", gameSetting->getNumberKunai());
	numberKunaiLabel->setString(numberKunai);

	if (gameSetting->getNumberKunai() <= 0)
	{
		kunaiButton->setBright(false);
		kunaiButton->setEnabled(false);
	}

	gameSetting->saveData();
}

void PlayScene::updateBloodSpell()
{
	auto heal = ParticleSystemQuad::create("particles/heal.plist");
	heal->setPosition(mainPlayer->getPosition());
	
	addChild(heal, 20);

	gameSetting->setBloodSpell(gameSetting->getBloodSpell() - 1);

	char str[20];
	sprintf(str, "%d", gameSetting->getBloodSpell());

	bloodSpellLabel->setString(str);

	mainPlayer->setCurrentBlood(mainPlayer->getCurrentBlood() + 20);

	if (gameSetting->getBloodSpell() <= 0)
	{
		spellButton->setBright(false);
		spellButton->setEnabled(false);
	}
}

void PlayScene::updatePowerBar(int percent)
{
	powerBar->setPercent(powerBar->getPercent() + percent);

	if (powerBar->getPercent() >= 100)
	{
		untyButton->setBright(true);
		untyButton->setEnabled(true);
	}
}

void PlayScene::showItem(float x, float y, int type)
{

	for (int i = 0; i < items.size(); i++)
	{
		if (!items.at(i)->getIsDislay() && items.at(i)->getTypeItem() == type)
		{
			items.at(i)->setIsDislay(true);
			items.at(i)->setPosition(x, y);

			if (type == COINS_ITEM)
			{
				items.at(i)->monneyAnimation(moneyAnimate);
			}

			break;
		}
	}
}

void PlayScene::playerCollectItem(int index)
{
	items.at(index)->setIsDislay(false);

	switch (items.at(index)->getTypeItem())
	{
	case COINS_ITEM:
		updateMoney(items.at(index)->getValue());
		break;
	default:
		break;
	}
}

void PlayScene::playerCollectExp(int exp, cocos2d::Vec2 position)
{
	expLabel->setOpacity(100.0f);

	expLabel->setVisible(true);
	expLabel->setPosition(position);

	expLabel->stopAllActions();

	auto moveBy = MoveBy::create(0.8f, Vec2(0, 100));
	auto fadeOut = FadeOut::create(0.8f);

	auto spawn = Spawn::create(moveBy, fadeOut, nullptr);

	expLabel->runAction(spawn);

	gameSetting->setEXP(gameSetting->getEXP() + exp);

	if (gameSetting->getEXP() >= gameSetting->getPlayerLevel() * 50)
	{
		mainPlayer->upLevel();

		char str[20];
		sprintf(str, "Lv %d", gameSetting->getPlayerLevel());

		levelLabel->setString(str);
	}

	expBar->setPercent((float)gameSetting->getEXP() / (gameSetting->getPlayerLevel() * 50)*100.0f);

}

void PlayScene::openGate()
{
	auto func1 = CallFunc::create([&]()
	{
		auto spriteFrameCache = SpriteFrameCache::getInstance();
		Vector<SpriteFrame*> listImage;
		char str[100];
		for (int i = 0; i < 4; i++)
		{
			sprintf(str, "lv1_gate%d.png", i);
			listImage.pushBack(spriteFrameCache->getSpriteFrameByName(str));
		}

		auto animation = Animation::createWithSpriteFrames(listImage, 0.1f);

		auto gateAction = Animate::create(animation);

		gate->runAction(gateAction);
	});

	auto func2 = CallFunc::create([&]()
	{
		auto montersScene = MonsterSecen::createScene();
		Director::getInstance()->replaceScene(TransitionFade::create(1.0f, montersScene));
	});

	auto sequence = Sequence::create(func1,DelayTime::create(0.6f),func2, nullptr);
	gate->runAction(sequence);

}