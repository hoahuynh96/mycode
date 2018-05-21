#include "GUI\HelpScene.h"
#include "GUI\MainMenuScene.h"
#include <ui/CocosGUI.h>
#include "Define.h"

USING_NS_CC;

Scene* HelpScene::createScene()
{
	auto scene = Scene::create();
	auto layer = HelpScene::create();
	scene->addChild(layer);
	return scene;
}
bool HelpScene::init()
{
	if (!Layer::init())
	{
		return false;
	}
	auto screenSize = Director::getInstance()->getVisibleSize();
	auto logo = Sprite::create("backgrounds/BackgroundInforActorScene.jpg");
	logo->setPosition(screenSize.width / 2, screenSize.height / 2);
	logo->setScale(SCENE_W / logo->getContentSize().width, SCENE_H/logo->getContentSize().height);
	addChild(logo);

	auto backButton = ui::Button::create();
	backButton->loadTextureNormal("backnormal.png", ui::Widget::TextureResType::PLIST);
	backButton->loadTexturePressed("backpress.png", ui::Widget::TextureResType::PLIST);
	backButton->addClickEventListener([&](Ref* event)
	{
		Director::getInstance()->replaceScene(TransitionFadeTR::create(0.5f, MainMenuScene::createScene()));
	});
	backButton->setAnchorPoint(Vec2(1, 1));
	backButton->setPosition(Vec2(90, 470));
	addChild(backButton, 2);

	auto skillLabel = Label::create("BUTTON", "fonts/Vnbodo.ttf", 24);
	skillLabel->enableOutline(Color4B::RED, 1);
	skillLabel->setPosition(180, 400);
	addChild(skillLabel, 1);

	auto leftSprite = Sprite::createWithSpriteFrameName("turnleftnormal.png");
	leftSprite->setPosition(30, 350);
	leftSprite->setScale(0.5);
	addChild(leftSprite, 1);
	auto rightSprite = Sprite::createWithSpriteFrameName("turnrightnormal.png");
	rightSprite->setPosition(80, 350);
	rightSprite->setScale(0.5);
	addChild(rightSprite, 1);

	auto moveLabel = Label::create("Move to Left / Right", "fonts/Vnbodo.ttf", 16);
	moveLabel->setPosition(200, 350);
	addChild(moveLabel, 1);

	auto chemSprite = Sprite::createWithSpriteFrameName("attacknormal.png");
	chemSprite->setPosition(60, 300);
	chemSprite->setScale(0.8);
	addChild(chemSprite, 1);
	auto chemLabel = Label::create("Attack Basic", "fonts/Vnbodo.ttf", 16);
	chemLabel->setPosition(170, 300);
	addChild(chemLabel, 1);

	auto jumpSprite = Sprite::createWithSpriteFrameName("jumpnormal.png");
	jumpSprite->setPosition(60, 250);
	jumpSprite->setScale(0.8);
	addChild(jumpSprite, 1);
	auto jumpLabel = Label::create("Jump", "fonts/Vnbodo.ttf", 16);
	jumpLabel->setPosition(140, 250);
	addChild(jumpLabel, 1);

	auto slideSprite = Sprite::createWithSpriteFrameName("slidenormal.png");
	slideSprite->setPosition(60, 200);
	slideSprite->setScale(0.8);
	addChild(slideSprite, 1);
	auto slideLabel = Label::create("Slide", "fonts/Vnbodo.ttf", 16);
	slideLabel->setPosition(140, 200);
	addChild(slideLabel, 1);

	auto bigbangSprite = Sprite::create("ui/bangnormal.png");
	bigbangSprite->setPosition(60, 150);
	bigbangSprite->setScale(0.35);
	addChild(bigbangSprite, 1);
	auto bigbangLabel = Label::create("Big Bang Skill", "fonts/Vnbodo.ttf", 16);
	bigbangLabel->setPosition(180, 150);
	addChild(bigbangLabel, 1);

	auto howplayLabel = Label::create("HOW TO PLAY?", "fonts/Vnbodo.ttf", 24);
	howplayLabel->enableOutline(Color4B::RED, 1);
	howplayLabel->setPosition(500, 400);
	addChild(howplayLabel, 1);

	auto tutorialLabel = Label::create("You must defeat boss to complete the level.\nThe coin will drop when you\nkill the monster. The coins are used to\nbuy stronger skill, health, defense and\nstrong. Use stronger skills to play at\nthe next level", "fonts/Vnbodo.ttf", 16);
	tutorialLabel->setPosition(500, 320);
	addChild(tutorialLabel, 1);

	scheduleUpdate();
	return true;
}