#include "GUI\MonsterSecen.h"
#include "GUI\WaitScene.h"
#include "GUI\GameOverScene.h"
#include "GUI\PlayScene.h"
#include "GUI\WinScene.h"
#include <stdio.h>
#include <stdlib.h>
#include "Define.h"
using namespace std;

USING_NS_CC;


extern int gCount;

Scene* MonsterSecen::createScene()
{
	auto scene = Scene::createWithPhysics();
	//scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	scene->getPhysicsWorld()->setGravity(Vec2(0.0f, -820.0f));

	auto layer = MonsterSecen::create();
	scene->addChild(layer);
	return scene;
}

bool MonsterSecen::init()
{
	if (!Layer::init())
	{
		return false;
	}

	ultiList.clear();
	for (int i = 0; i < 1; i++)
	{
		ultiList.push_back(new Ultimatel(this));
	}

	gCount = 0;

	visibleSize = Director::getInstance()->getVisibleSize();
	gameSetting = GameSetting::getInstance();

	auto backgound = Sprite::create("backgrounds/background_boss.png");
	backgound->setPosition(Vec2(0.0f, 0.0f));
	backgound->setAnchorPoint(Vec2(0.0f, 0.0f));
	addChild(backgound, -2);

	physicsTileMap = TMXTiledMap::create("titlemaps/demo.tmx");
	addChild(physicsTileMap, 1);

	auto objectLayer = physicsTileMap->getObjectGroup("object");

	auto objects = objectLayer->getObject("player");
	int x = objects.at("x").asInt();
	int y = objects.at("y").asInt();

	mainPlayer = new Player(this);
	mainPlayer->setAnchorPoint(Vec2(0.0f, 0.0f));
	mainPlayer->setPosition(Vec2((float)x, (float)y));

	auto objects1 = objectLayer->getObject("crep");
	int x3 = objects1.at("x").asInt();
	int y3 = objects1.at("y").asInt();

	dragon = new Dragon(this);
	dragon->setAnchorPoint(Vec2(0.0f, 0.0f));
	dragon->setPosition(Vec2((float)x3, (float)y3));


	auto objectsBoss = objectLayer->getObject("boss");
	int x1 = objectsBoss.at("x").asInt();
	int y1 = objectsBoss.at("y").asInt();

	
	mosterBoss = new MosterBoss(this);
	mosterBoss->setAnchorPoint(Vec2(0.0f, 0.0f));
	mosterBoss->setPosition(Vec2((float)x1, (float)y1));

	auto objects2 = objectLayer->getObject("crep1");
	int x4 = objects2.at("x").asInt();
	int y4 = objects2.at("y").asInt();

	crep = new Crep(this);
	crep->setAnchorPoint(Vec2(0.0f, 0.0f));
	crep->setPosition(Vec2((float)x4, (float)y4));

	createUi();

	createButton();

	createPhysic();

//	createItem();

	auto touchEvent = EventListenerTouchOneByOne::create();

	touchEvent->onTouchBegan = CC_CALLBACK_2(MonsterSecen::onTouchBegan, this);
	touchEvent->onTouchMoved = CC_CALLBACK_2(MonsterSecen::onTouchMoved, this);
	touchEvent->onTouchEnded = CC_CALLBACK_2(MonsterSecen::onTouchEnded, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchEvent, this);

	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(MonsterSecen::onContactBegin, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

	scheduleUpdate();

	return true;
}

void MonsterSecen::update(float dt)
{
	
	if (mainPlayer->getCurrentBlood() <= 0 || mainPlayer->getPositionY() <= 0)
	{
		auto gameOverScene = GameOverScene::createScene();
		Director::getInstance()->replaceScene(gameOverScene);
	}


	// neu rong het mau thi chet
	if (dragon->getCurrentBlood() <= 0)
	{
		dragon->setIsDislay(false);
		dragon->setIsAlive(false);
	}
	
	if (crep->getCurrentBlood() <= 0)
	{
		crep->setIsDislay(false);
		crep->setIsAlive(false);
	}

	if (mosterBoss->getCurrentBlood() <= 0)
	{
		gameSetting->setLevel(gameSetting->getLevel() + 1);
		auto montersScene = WinScene::createScene();
		Director::getInstance()->replaceScene(montersScene); 
	}
	
	setCenterView();

	dragon->fly();
	mainPlayer->update();

	updateEnemy();

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

void MonsterSecen::moveLeft()
{
	float x = 0;

	//neu vi tri cua nhan vat chinh con lon hon 0.2 man hinh, hoac lon hon 0 va map da den diem dau tien thi cho phep di chuyen
	if (mainPlayer->getPositionX() > visibleSize.width*0.2 || (mainPlayer->getPositionX()>0 && physicsTileMap->getPositionX() >= 0))
	{
		x = mainPlayer->getPositionX();
		mainPlayer->setPositionX(x - 4);
	}
	else
	{
		if (physicsTileMap->getPositionX() < 0)
		{
			physicsTileMap->setPositionX(physicsTileMap->getPositionX() + 4);
		}
	}
}

void MonsterSecen::moveRight()
{
	float x;

	if (mainPlayer->getPositionX() < visibleSize.width / 2 || (mainPlayer->getPositionX() < visibleSize.width - 40 && physicsTileMap->getPositionX() + physicsTileMap->getContentSize().width <= visibleSize.width))
	{
		float x = mainPlayer->getPositionX();
		mainPlayer->setPositionX(x + 4);
	}
	else
	{
		if (physicsTileMap->getPositionX() + physicsTileMap->getContentSize().width > visibleSize.width)
		{
			physicsTileMap->setPositionX(physicsTileMap->getPositionX() - 4);
			
		}
	}
}

bool MonsterSecen::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
	if (mainPlayer->getCurrentAction() != SLIDE)
	{
		if (moveLeftNormal->getBoundingBox().containsPoint(touch->getLocation()))
		{
			moveLeftPress->setVisible(true);
			mainPlayer->turnLeft();

			if (mainPlayer->getDirection() == TURNLEFT)
			{
				mainPlayer->setIsMove(true);
			}
		}

		if (moveRightNormal->getBoundingBox().containsPoint(touch->getLocation()))
		{
			moveRightPress->setVisible(true);
			mainPlayer->turnRight();

			if (mainPlayer->getDirection() == TURNRIGHT)
			{
				mainPlayer->setIsMove(true);
			}
		}
	}

	return true;
}

void MonsterSecen::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event)
{

	if (mainPlayer->getCurrentAction() == SLIDE && mainPlayer->getCurrentAction() > 0)
	{

	}
	else
	{
		if (moveLeftNormal->getBoundingBox().containsPoint(touch->getLocation()))
		{
			moveLeftPress->setVisible(true);
			mainPlayer->turnLeft();

			if (mainPlayer->getDirection() == TURNLEFT)
			{
				mainPlayer->setIsMove(true);
			}
		}
		else if (moveRightNormal->getBoundingBox().containsPoint(touch->getLocation()))
		{
			moveRightPress->setVisible(true);
			mainPlayer->turnRight();

			if (mainPlayer->getDirection() == TURNRIGHT)
			{
				mainPlayer->setIsMove(true);
			}

		}
		else
		{
			moveLeftPress->setVisible(false);
			moveRightPress->setVisible(false);

			/*if (mainPlayer->getCurrentAction() != JUMP)
			{
			mainPlayer->Idle();
			}*/

			mainPlayer->setIsMove(false);
		}
	}
}

void MonsterSecen::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event)
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

void MonsterSecen::setCenterView()
{
	// neu nhan vat dang di chuyen thi thuc hien
	if (mainPlayer->getIsMove())
	{
		if (mainPlayer->getDirection() == TURNLEFT)
		{
			moveLeft();
		}
		else
		{
			moveRight();
		}

	}
}

bool MonsterSecen::onContactBegin(cocos2d::PhysicsContact& contact)
{

	//Neu kieu tra la false thi di xuyen qua vat the

	auto shapeA = contact.getShapeA()->getBody();
	auto shapeB = contact.getShapeB()->getBody();

	//neu Player va cham monter thi khong xet su kien

	if (shapeA->getCollisionBitmask() == PLAYER_BISMARK && shapeB->getCollisionBitmask() == CREP_BISMARK || shapeA->getCollisionBitmask() == CREP_BISMARK && shapeB->getCollisionBitmask() == PLAYER_BISMARK)
	{
		return false;
	}

	if (shapeA->getCollisionBitmask() == PLAYER_BISMARK && shapeB->getCollisionBitmask() == BOSS_BISMARK || shapeA->getCollisionBitmask() == BOSS_BISMARK && shapeB->getCollisionBitmask() == PLAYER_BISMARK)
	{
		return false;
	}
	if (shapeA->getCollisionBitmask() == CREP_BISMARK && shapeB->getCollisionBitmask() == BOSS_BISMARK || shapeA->getCollisionBitmask() == BOSS_BISMARK && shapeB->getCollisionBitmask() == CREP_BISMARK)
	{
		return false;
	}


	//Xet player o tren mat dat

	if (shapeA->getCollisionBitmask() == PLAYER_BISMARK && shapeB->getCollisionBitmask() == ROUND_BISMARK)
	{
		if (shapeA->getPosition().y > shapeB->getPosition().y + 30.0f)
		{
			mainPlayer->setIsOnland(true);

		}
	}
	else if (shapeA->getCollisionBitmask() == ROUND_BISMARK && shapeB->getCollisionBitmask() == PLAYER_BISMARK)
	{
		if (shapeA->getPosition().y + 30.0f < shapeB->getPosition().y)
		{
			mainPlayer->setIsOnland(true);
		}
	}

	// Neu playler cham thung go thi cho phep nhay tiep
	if (shapeA->getCollisionBitmask() == PLAYER_BISMARK && shapeB->getCollisionBitmask() == WOOD_BISMARK)
	{
		if (shapeA->getPosition().y > shapeB->getPosition().y)
		{
			mainPlayer->setIsOnland(true);

		}
	}
	else if (shapeA->getCollisionBitmask() == WOOD_BISMARK && shapeB->getCollisionBitmask() == PLAYER_BISMARK)
	{
		if (shapeA->getPosition().y < shapeB->getPosition().y)
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

		mainPlayer->setCurrentBlood(mainPlayer->getCurrentBlood() - 10);
	}
	else if (shapeA->getCollisionBitmask() == BOMP_BISMARK && shapeB->getCollisionBitmask() == PLAYER_BISMARK)
	{

		auto explode = ParticleSystemQuad::create("particles/particle_texture.plist");
		explode->setPosition(shapeA->getPosition());

		addChild(explode);

		physicsTileMap->removeChild(shapeA->getNode());
		mainPlayer->setCurrentBlood(mainPlayer->getCurrentBlood() - 10);
	}

	//Xet dan cua rong va cham vs nhan vat

	if (shapeA->getCollisionBitmask() == BULLETDRAGON_BISMARK)
	{
		
		dragon->hasCollision();
		if (shapeB->getCollisionBitmask() == PLAYER_BISMARK)
		{
			mainPlayer->beingAttacked(25);
			shapeA->setEnabled(false);
			updatePowerBar();
		}
		else
		{
			return false;
		}
	}

	if (shapeB->getCollisionBitmask() == BULLETDRAGON_BISMARK)
	{
		dragon->hasCollision();
		if (shapeA->getCollisionBitmask() == PLAYER_BISMARK)
		{
			mainPlayer->beingAttacked(20);
			shapeB->setEnabled(false);
			
			updatePowerBar();
		}
		else
		{
			return false;
		}
	}
	//Xet dan phi tieu trung boss

	if (shapeA->getCollisionBitmask() == KUNAI_BISMARK)
	{
		dragon->hasCollision();
		if (shapeB->getCollisionBitmask() == BOSS_BISMARK)
		{
			mainPlayer->hasConclusion(shapeA->getGroup());
			auto bloodparticle = ParticleSystemQuad::create("particles/particle_blood.plist");
			bloodparticle->setPosition(shapeB->getPosition());

			addChild(bloodparticle, 20);

			mosterBoss->setCurrentBlood(mosterBoss->getCurrentBlood() - 10);
			shapeA->setEnabled(false);
		}
		else
		{
			return false;
		}
	}

	if (shapeB->getCollisionBitmask() == KUNAI_BISMARK)
	{
		dragon->hasCollision();
		mainPlayer->hasConclusion(shapeB->getGroup());
		if (shapeA->getCollisionBitmask() == BOSS_BISMARK)
		{

			auto bloodparticle = ParticleSystemQuad::create("particles/particle_blood.plist");
			bloodparticle->setPosition(shapeA->getPosition());

			addChild(bloodparticle, 20);

			mosterBoss->setCurrentBlood(mosterBoss->getCurrentBlood() - 10);
			shapeB->setEnabled(false);
		}
		else
		{
			return false;
		}
	}
	//Xet dan phi tieu trung rong

	if (shapeA->getCollisionBitmask() == KUNAI_BISMARK)
	{
		dragon->hasCollision();
		if (shapeB->getCollisionBitmask() == DRAGON_BISMARK)
		{

			auto bloodparticle = ParticleSystemQuad::create("particles/particle_blood.plist");
			bloodparticle->setPosition(shapeB->getPosition());

			addChild(bloodparticle, 20);

			dragon->setCurrentBlood(dragon->getCurrentBlood() - 10);
			shapeA->setEnabled(false);
		}
		else
		{
			return false;
		}
	}

	if (shapeB->getCollisionBitmask() == KUNAI_BISMARK)
	{
		dragon->hasCollision();
		if (shapeA->getCollisionBitmask() == DRAGON_BISMARK)
		{

			auto bloodparticle = ParticleSystemQuad::create("particles/particle_blood.plist");
			bloodparticle->setPosition(shapeA->getPosition());

			addChild(bloodparticle, 20);

			dragon->setCurrentBlood(dragon->getCurrentBlood() - 10);
			shapeB->setEnabled(false);
		}
		else
		{
			return false;
		}
	}

	//xet phi tieu trung crep
	if (shapeA->getCollisionBitmask() == KUNAI_BISMARK)
	{
		if (shapeB->getCollisionBitmask() == CREP_BISMARK)
		{

			auto bloodparticle = ParticleSystemQuad::create("particles/particle_blood.plist");
			bloodparticle->setPosition(shapeB->getPosition());

			addChild(bloodparticle, 20);

			crep->setCurrentBlood(crep->getCurrentBlood() - 10);
			shapeA->setEnabled(false);
		}
		else
		{
			return false;
		}
	}

	if (shapeB->getCollisionBitmask() == KUNAI_BISMARK)
	{
		if (shapeA->getCollisionBitmask() == CREP_BISMARK)
		{

			auto bloodparticle = ParticleSystemQuad::create("particles/particle_blood.plist");
			bloodparticle->setPosition(shapeA->getPosition());

			addChild(bloodparticle, 20);

			crep->setCurrentBlood(crep->getCurrentBlood() - 10);
			shapeB->setEnabled(false);
		}
		else
		{
			return false;
		}
	}
	
	// ulti trung nhan vat
	if (shapeA->getCollisionBitmask() == ULTIDRAGON_BISMARK)
	{
		
		if (shapeB->getCollisionBitmask() == PLAYER_BISMARK)
		{
			mainPlayer->beingAttacked(35);
	
			shapeA->setEnabled(false);
			shapeA->getNode()->setVisible(false);

			updatePowerBar();

		}
		else
		{
			return false;
		}
	}

	if (shapeB->getCollisionBitmask() == ULTIDRAGON_BISMARK)
	{
		
		if (shapeA->getCollisionBitmask() == PLAYER_BISMARK)
		{
			mainPlayer->beingAttacked(35);
			shapeB->setEnabled(false);
			shapeB->getNode()->setVisible(false);
			updatePowerBar();
		}
		else
		{
			return false;
		}
	}

	//Player chem trung monters
	if (shapeA->getCollisionBitmask() == PLAYER_DAMAGE_BISMARK)
	{
		if (shapeB->getCollisionBitmask() == BOSS_BISMARK)
		{
			shapeA->setEnabled(false);
			mosterBoss->setCurrentBlood(mosterBoss->getCurrentBlood() - 10);
			
		}
		else if (shapeB->getCollisionBitmask() == DRAGON_BISMARK)
		{
			shapeA->setEnabled(false);
			dragon->setCurrentBlood(dragon->getCurrentBlood() - 10);
		}
		else if (shapeB->getCollisionBitmask() == CREP_BISMARK)
		{
			shapeA->setEnabled(false);
			crep->setCurrentBlood(crep->getCurrentBlood() - 10);
		}
		else
		{
			return false;
		}
	}

	if (shapeB->getCollisionBitmask() == PLAYER_DAMAGE_BISMARK)
	{
		if (shapeA->getCollisionBitmask() == BOSS_BISMARK)
		{
			shapeB->setEnabled(false);
			mosterBoss->setCurrentBlood(mosterBoss->getCurrentBlood() - 10);
			
		}
		else if (shapeA->getCollisionBitmask() == DRAGON_BISMARK)
		{
			shapeB->setEnabled(false);
			dragon->setCurrentBlood(dragon->getCurrentBlood() - 10);
			
		}
		else if (shapeA->getCollisionBitmask() == CREP_BISMARK)
		{
			shapeB->setEnabled(false);
			crep->setCurrentBlood(crep->getCurrentBlood() - 10);
		}
		else
		{
			return false;
		}
	}

	//boss chem trung player
	if (shapeA->getCollisionBitmask() == MONTER_DAMAGE_BISMARK)
	{
		if (shapeB->getCollisionBitmask() == PLAYER_BISMARK)
		{
			mainPlayer->beingAttacked(10);
			shapeA->setEnabled(false);
			updatePowerBar();
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
			mainPlayer->beingAttacked(10);
			shapeB->setEnabled(false);
			updatePowerBar();
		}
		else
		{
			return false;
		}
	}

	//Xet powerSword va cham vat ly
	if (shapeA->getCollisionBitmask() == POWER_BISMARK)
	{
		
		if (shapeB->getCollisionBitmask() == BOSS_BISMARK)
		{
			mosterBoss->setCurrentBlood(mosterBoss->getCurrentBlood() - 30);
			shapeA->setEnabled(false);
		}
		else if (shapeB->getCollisionBitmask() == DRAGON_BISMARK)
		{
			dragon->setCurrentBlood(dragon->getCurrentBlood() - 30);
			shapeA->setEnabled(false);
		}
		else if (shapeB->getCollisionBitmask() == CREP_BISMARK)
		{
			crep->setCurrentBlood(crep->getCurrentBlood() - 30);
			shapeA->setEnabled(false);
		}
		else
		{
			return false;
		}
	}
	else if (shapeB->getCollisionBitmask() == POWER_BISMARK)
	{
		
		if (shapeA->getCollisionBitmask() == BOSS_BISMARK)
		{
			mosterBoss->setCurrentBlood(mosterBoss->getCurrentBlood() - 30);
			shapeB->setEnabled(false);
		}
		else if (shapeA->getCollisionBitmask() == DRAGON_BISMARK)
		{
			dragon->setCurrentBlood(dragon->getCurrentBlood() - 30);
			shapeB->setEnabled(false);
		}
		else if (shapeA->getCollisionBitmask() == CREP_BISMARK)
		{
			crep->setCurrentBlood(crep->getCurrentBlood() - 30);
			shapeB->setEnabled(false);
		}
		else
		{
			return false;
		}
	}

	if (shapeA->getCollisionBitmask() == CREP_DAMAGE_BISMARK)
	{
		if (shapeB->getCollisionBitmask() == PLAYER_BISMARK)
		{
			mainPlayer->beingAttacked(30);

			shapeA->setEnabled(false);
			updatePowerBar();
		}
		else
		{
			return false;
		}
	}

	if (shapeB->getCollisionBitmask() == CREP_DAMAGE_BISMARK)
	{
		if (shapeA->getCollisionBitmask() == PLAYER_BISMARK)
		{
			mainPlayer->beingAttacked(30);
	
			shapeB->setEnabled(false);

			updatePowerBar();
		}
		else
		{
			return false;
		}
	}
	
	return true;
}

void MonsterSecen::createUi()
{
	char moneyValue[20];
	sprintf(moneyValue, "%d", gameSetting->getMoney());

	money = Label::create();
	money->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - 20.0f));
	money->setSystemFontSize(28.0f);
	money->setColor(Color3B::RED);
	money->setString(moneyValue);
	this->addChild(money);

	/*auto iconMoney = Sprite::create("items/money.png");
	iconMoney->setPosition(Vec2(visibleSize.width / 2 - 60.0f, visibleSize.height - 20.0f));
	this->addChild(iconMoney);*/
	powerBar = ui::LoadingBar::create();
	powerBar->loadTexture("powerBar.png", ui::Widget::TextureResType::PLIST);
	powerBar->setPercent(0.0f);
	powerBar->setPosition(Vec2(99.0f, 408.0f));
	powerBar->setAnchorPoint(Vec2(0.0f, 0.0f));
	powerBar->setDirection(ui::LoadingBar::Direction::LEFT);
	addChild(powerBar, 11);
}

void MonsterSecen::createButton()
{
	
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
	attackButton->setScale(1.2f);
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

void MonsterSecen::createPhysic()
{
	/*Tao physics cho Map vat ly*/
	auto layerPhysic = physicsTileMap->getLayer("physics");
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
				titleSet->setPhysicsBody(titleSetBody);
			}
		}
	}
	
}

void MonsterSecen::updateEnemy()
{
	gCount++;
	if (mainPlayer->getPosition().x - mosterBoss->getPosition().x < mosterBoss->getContentSize().width && mainPlayer->getPosition().x - mosterBoss->getPosition().x > -mainPlayer->getContentSize().width &&mainPlayer->getPositionY() - mosterBoss->getPositionY() <= 120)
	{
		mosterBoss->actack();
	}
	else
	{
		if (mosterBoss->getCurrentBlood() != 200 && mosterBoss->getCurrentBlood() != 130 && mosterBoss->getCurrentBlood() != 80)
		{
			
			mosterBoss->runBoss(mainPlayer->getPositionX(), mainPlayer->getPositionY());
		}
	
	}
	//------------------------------------
	if (crep->getIsDislay())
	{
		if (mainPlayer->getPosition().x - crep->getPosition().x < crep->getContentSize().width && mainPlayer->getPosition().x - crep->getPosition().x > -mainPlayer->getContentSize().width)
		{
			crep->actack();
		}
		else
		{
			{

				crep->runCrep(mainPlayer->getPositionX(), mainPlayer->getPositionY());
			}

		}
	}
	
	//---------------------------------------

	if (mosterBoss->getCurrentBlood() <= 200 && check==0)
	{
		check = 1;
		dragon->setIsDislay(true);
	}
	
	if (mosterBoss->getCurrentBlood() <= 120 && check1==0)
	{
		check1 = 1;
		crep->setIsDislay(true);
	}
	if (dragon->getIsDislay() == true)
	{
		if (gCount % 100 == 0)
		{
			dragon->Shooting(mainPlayer->getPositionX(), mainPlayer->getPositionY() + 30);
		}
		
	}
	dragon->update();

	
	if (mosterBoss->getCurrentBlood() == 200 || mosterBoss->getCurrentBlood() == 130 || mosterBoss->getCurrentBlood() == 80)
	{
		if (mosterBoss->getPositionX() < 600 && mosterBoss->getDirection()==TURNLEFT)
		{
			mosterBoss->runback();
		}
		if ( mosterBoss->getPositionX() < 602 && mosterBoss->getPositionX()>600)
		{
			
			if (gCount % 150 == 0)
			{
				
				
				for (Ultimatel* unti : ultiList)
				{
					{
					
						if (unti->getIsDislay() == true);
						{
							unti->fall();
							unti->setPosition(Vec2(mainPlayer->getPositionX() + random(-10, 120), 400));
							break;
						}
					
					}

				}
			mosterBoss->skill();
			}
			
			
		}
	}
	
	
	
}

void MonsterSecen::updateMoney(int money)
{
	gameSetting->setMoney(money);

	char moneyValue[20];
	sprintf(moneyValue, "%d", gameSetting->getMoney());

	this->money->setString(moneyValue);

	gameSetting->saveData();
}

void MonsterSecen::updateKunai()
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

void MonsterSecen::updateBloodSpell()
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

	gameSetting->saveData();
}

void MonsterSecen::updatePowerBar()
{
	powerBar->setPercent(powerBar->getPercent() + 10.0f);

	if (powerBar->getPercent() >= 100)
	{
		untyButton->setBright(true);
		untyButton->setEnabled(true);
	}
}