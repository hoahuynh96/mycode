#include "GUI\WaitScene.h"
#include "GUI\PlayScene.h"
#include "GUI\MainMenuScene.h"
#include "GUI\ShopItem.h"
#include "GUI\InforActorScene.h"
#include "Model\GameSetting.h"
#include "ui\CocosGUI.h"

#include "Define.h"

USING_NS_CC;

Scene* WaitScene::createScene()
{
	auto scene = Scene::create();
	auto layer = WaitScene::create();
	scene->addChild(layer);
	return scene;
}

bool WaitScene::init()
{

	if (!Layer::init())
	{
		return false;
	}

	//background
	auto screenSize = Director::getInstance()->getVisibleSize();
	auto map = Sprite::create("backgrounds/map.jpg");
	map->setPosition(screenSize.width / 2, screenSize.height / 2);
	map->setScale(SCENE_W / map->getContentSize().width, SCENE_H / map->getContentSize().height);
	addChild(map);



	auto visibleSize = Director::getInstance()->getVisibleSize();


	// bt lv1
	auto btLevel1 = ui::Button::create("ui/level1.png", "ui/level1-1.png");
	btLevel1->setPosition(Vec2(190, 220));
	btLevel1->setScale(0.6f);
	addChild(btLevel1, 5);
	btLevel1->addClickEventListener([=](Ref* event) {
		GameSetting::getInstance()->setLevel(1);
		auto playScene = PlayScene::createScene();
		Director::getInstance()->replaceScene(playScene);
		
	});

	// bt lv2
	auto btLevel2 = ui::Button::create("ui/level2.png", "ui/level2-2.png");
	btLevel2->setPosition(Vec2(270, 150));
	btLevel2->setScale(0.6f);
	addChild(btLevel2, 5);
	btLevel2->addClickEventListener([=](Ref* event) {
		GameSetting::getInstance()->setLevel(2);
		auto playScene = PlayScene::createScene();
		Director::getInstance()->replaceScene(playScene);
	});


	// bt lv3
	auto btLevel3 = ui::Button::create("ui/level3.png", "ui/level3-2.png");
	btLevel3->setPosition(Vec2(370, 120));
	btLevel3->setScale(0.6f);
	addChild(btLevel3, 5);
	/*btLevel3->addClickEventListener([=](Ref* event) {

		auto playScene = PlayScene::createScene();
		Director::getInstance()->replaceScene(playScene);

	});*/


	// bt lv4
	auto btLevel4 = ui::Button::create("ui/level3.png", "ui/level4-2.png");
	btLevel4->setPosition(Vec2(490, 115));
	btLevel4->setScale(0.6f);
	addChild(btLevel4, 5);
	/*btLevel4->addClickEventListener([=](Ref* event) {

		auto playScene = PlayScene::createScene();
		Director::getInstance()->replaceScene(playScene);

	});*/



	// bt lv5
	auto btLevel5 = ui::Button::create("ui/level3.png", "ui/level5-2.png");
	btLevel5->setPosition(Vec2(520, 210));
	btLevel5->setScale(0.6f);
	addChild(btLevel5, 5);
	/*btLevel5->addClickEventListener([=](Ref* event) {
		auto playScene = PlayScene::createScene();
		Director::getInstance()->replaceScene(playScene);


	});*/

	//them button shop
	auto shopButton = ui::Button::create("ui/shopnormal.png", "ui/shoppress.png");
	shopButton->setAnchorPoint(Vec2(1.0f, 1.0f));
	shopButton->setPosition(Vec2(visibleSize));
	addChild(shopButton);
	shopButton->addClickEventListener([&](Ref* render){
		GameSetting::getInstance()->back = 1;
		auto scene = ShopItem::createScene();
		Director::getInstance()->replaceScene(scene);
	});

	// them back button
	auto backButton = ui::Button::create();
	backButton->loadTextureNormal("backnormal.png", ui::Widget::TextureResType::PLIST);
	backButton->loadTexturePressed("backpress.png", ui::Widget::TextureResType::PLIST);
	backButton->setPosition(Vec2(50.0f, 50.0f));
	addChild(backButton);
	backButton->addClickEventListener([&](Ref* render){
		Director::getInstance()->replaceScene(TransitionFlipAngular::create(0.5f, MainMenuScene::createScene()));
	});

	auto infoplayer = ui::Button::create("ui/inforplayer.png");
	infoplayer->setPosition(Vec2(50.0f, 430.0f));
	addChild(infoplayer);
	infoplayer->addClickEventListener([&](Ref* render)
	{
		auto infoScene = InforActorScene::createScene();
		Director::getInstance()->replaceScene(TransitionFlipAngular::create(0.5f, infoScene));
	});

	return true;
}