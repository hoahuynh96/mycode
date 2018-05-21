#include "GUI\InforActorScene.h"
#include "GUI\MainMenuScene.h"
#include "Model\GameSetting.h"
#include <ui/CocosGUI.h>
#include "Define.h"

USING_NS_CC;

Scene* InforActorScene::createScene()
{
	auto scene = Scene::create();
	auto layer = InforActorScene::create();
	scene->addChild(layer);
	return scene;
}
bool InforActorScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	char str[40];

	GameSetting* gameSetting = GameSetting::getInstance();
	auto screenSize = Director::getInstance()->getVisibleSize();

	auto logo = Sprite::create("backgrounds/BackgroundInforActorScene.jpg");
	logo->setScale(SCENE_W / logo->getContentSize().width, SCENE_H / logo->getContentSize().height);
	logo->setPosition(screenSize.width / 2, screenSize.height / 2);
	addChild(logo);

	//Actor
	auto Actor = Sprite::create("player/play.png");
	Actor->setPosition(200, screenSize.height / 2);
	Actor->setScale(0.5f, 0.5f);
	addChild(Actor, 1);

	//Name
	auto Nathan = Sprite::create("ui/NameActor.png");
	Nathan->setPosition(200, 100);
	Nathan->setScale(0.8);
	addChild(Nathan, 1);

	//BackButton
	auto backButton = ui::Button::create();
	backButton->loadTextureNormal("backnormal.png", ui::Widget::TextureResType::PLIST);
	backButton->loadTexturePressed("backpress.png", ui::Widget::TextureResType::PLIST);
	backButton->addClickEventListener([&](Ref* event)
	{
		Director::getInstance()->replaceScene(TransitionFadeTR::create(0.5f, MainMenuScene::createScene()));
	});
	backButton->setPosition(Vec2(45.0f, 430.0f));
	addChild(backButton, 1);

	//Level label
	sprintf(str, "Level %d", gameSetting->getPlayerLevel());

	auto levelLabel = Label::create(str, "fonts/Vnaris.ttf", 30);
	levelLabel->setPosition(200.0f, 320.0f);
	addChild(levelLabel, 2);

	//Chacracter Player
	auto sceneLabel = Label::create("CHACRACTER PLAYER", "fonts/Vncoop.ttf", 35);
	sceneLabel->setColor(Color3B::GRAY);
	sceneLabel->enableOutline(Color4B::BLACK, 2);
	sceneLabel->setPosition(screenSize.width / 2, 450);
	addChild(sceneLabel, 1);

	sprintf(str, "%d", gameSetting->getBloodPlayer());

	//Health Icon
	auto healthIcon = Sprite::create("ui/Health.png");
	healthIcon->setPosition(Vec2(60.0f, 280.0f));
	healthIcon->setScale(0.3f, 0.3f);
	addChild(healthIcon, 1);
	auto healthLabel1 = Label::createWithTTF(str, "fonts/Vnaris.ttf", 20);
	healthLabel1->setPosition(Vec2(90.0f, 280.0f));
	healthLabel1->setAnchorPoint(Vec2(0.0f, 0.5f));
	addChild(healthLabel1, 1);

	sprintf(str, "%d", gameSetting->getArmor());

	//DefenseIcon
	auto defenseIcon = Sprite::create("ui/Giap.png");
	defenseIcon->setPosition(Vec2(60.0f, 240.0f));
	defenseIcon->setScale(0.3, 0.3);
	addChild(defenseIcon, 1);
	auto defenseIcon1 = Label::createWithTTF(str, "fonts/Vnaris.ttf", 22);
	defenseIcon1->setPosition(Vec2(90.0f, 240.0f));
	defenseIcon1->setAnchorPoint(Vec2(0.0f, 0.5f));
	addChild(defenseIcon1, 1);

	sprintf(str, "%d", gameSetting->getDamgePlayer());
	//Strong Icon
	auto strongIcon = Sprite::create("ui/Strong.png");
	strongIcon->setPosition(Vec2(60.0f, 200.0f));
	strongIcon->setScale(0.3, 0.3);
	addChild(strongIcon, 1);
	auto strongIcon1 = Label::createWithTTF(str, "fonts/Vnaris.ttf", 20);
	strongIcon1->setPosition(Vec2(90.0f, 200.0f));
	strongIcon1->setAnchorPoint(Vec2(0.0f, 0.5f));
	addChild(strongIcon1, 1);

	//Skill Label
	auto skillLabel = Label::create("Knife Skill", "fonts/Vnaris.ttf", 40);
	skillLabel->enableOutline(Color4B::RED, 2);
	skillLabel->setPosition(500, 350);
	addChild(skillLabel, 1);

	//Strong Label
	sprintf(str, "Damage: %d", gameSetting->getDamgePlayer());
	auto strongLabel = Label::create(str, "fonts/Vnaris.ttf", 30);
	strongLabel->setPosition(500, 300);
	addChild(strongLabel, 1);

	//Recovery Label
	auto recoveryLabel = Label::create("Recovery Time: 0s", "fonts/Vnaris.ttf", 30);
	recoveryLabel->setPosition(500, 250);
	addChild(recoveryLabel, 1);

	//Number of Darts
	/*auto numberdartsLabel = Label::create("Number of Darts: 10/10", "fonts/Vnaris.ttf", 30);
	numberdartsLabel->setPosition(500, 200);
	numberdartsLabel->setVisible(false);
	addChild(numberdartsLabel, 1);*/

	//Unlock Label
	//auto unlockLabel = Label::create("Unlock: 0", "fonts/Vnaris.ttf", 30);
	//unlockLabel->setPosition(500, 200);
	//addChild(unlockLabel, 1);

	//Money Icon
	/*auto moneyIcon = Sprite::createWithSpriteFrameName("iconmoney.png");
	moneyIcon->setPosition(600, 200);
	moneyIcon->setScale(1.0f);
	addChild(moneyIcon, 2);*/

	//Chem Button
	auto chemButton = ui::Button::create();
	chemButton->loadTextureNormal("attacknormal.png", ui::Widget::TextureResType::PLIST);
	chemButton->loadTexturePressed("attackpress.png", ui::Widget::TextureResType::PLIST);
	chemButton->addClickEventListener([=](Ref* event)
	{
		skillLabel->setString("Darts Shot Skill");
		strongLabel->setString("Damage: 10");
		//numberdartsLabel->setVisible(true);
		//unlockLabel->setString("Unlock: 200");
		//unlockLabel->setPosition(500, 150);
		//moneyIcon->setPosition(600, 150);
	});
	chemButton->setPosition(Vec2(300, 350));
	addChild(chemButton, 1);

	//Phóng phi tieu Button
	auto phitieuButton = ui::Button::create();
	phitieuButton->loadTextureNormal("kunainormal.png", ui::Widget::TextureResType::PLIST);
	phitieuButton->loadTexturePressed("kunaipress.png", ui::Widget::TextureResType::PLIST);
	phitieuButton->addClickEventListener([=](Ref* event)
	{

		skillLabel->setString("Knife Skill");
		strongLabel->setString("Damage: 120% dmg");
		recoveryLabel->setString("increase by level");
		//numberdartsLabel->setVisible(false);
		//unlockLabel->setString("Unlock: 0");
		//unlockLabel->setPosition(500, 200);
		//moneyIcon->setPosition(600, 200);
	});
	phitieuButton->setPosition(Vec2(320, 280));
	addChild(phitieuButton, 1);

	//Slide Button
	auto slideButton = ui::Button::create();
	slideButton->loadTextureNormal("slidenormal.png", ui::Widget::TextureResType::PLIST);
	slideButton->loadTexturePressed("slidepress.png", ui::Widget::TextureResType::PLIST);
	slideButton->addClickEventListener([=](Ref* event)
	{
		skillLabel->setString("Slide Skill");
		strongLabel->setString("Damage: 0 dmg");
		recoveryLabel->setString("");
		//numberdartsLabel->setVisible(false);
		//unlockLabel->setString("Unlock: 300");
		//unlockLabel->setPosition(500, 200);
		//moneyIcon->setPosition(600, 200);
	});
	slideButton->setPosition(Vec2(320, 210));
	addChild(slideButton, 1);

	//Bang button
	auto untyButton = ui::Button::create();
	untyButton->loadTextureNormal("untynormal.png", ui::Widget::TextureResType::PLIST);
	untyButton->loadTexturePressed("untypress.png", ui::Widget::TextureResType::PLIST);
	untyButton->addClickEventListener([=](Ref* event)
	{
		skillLabel->setString("BigBang Skill");
		strongLabel->setString("Damage: 300%");
		recoveryLabel->setString("increase by level");
		//numberdartsLabel->setVisible(false);
		//unlockLabel->setString("Unlock: 500");
		//unlockLabel->setPosition(500, 200);
		//moneyIcon->setPosition(600, 200);
	});
	untyButton->setPosition(Vec2(310, 140));
	addChild(untyButton, 1);

	scheduleUpdate();
	return true;
}