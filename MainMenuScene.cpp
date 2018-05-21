#include "GUI\MainMenuScene.h"
#include "GUI\WaitScene.h"
#include "GUI\AboutScene.h"
#include "GUI\HelpScene.h"
#include "GUI\SettingScene.h"
#include "GUI\Intro.h"
#include "GUI\ShopItem.h"
#include "Define.h"


USING_NS_CC;
Scene* MainMenuScene::createScene()
{
	auto scene = Scene::create();
	auto layer = MainMenuScene::create();
	scene->addChild(layer);
	return scene;
}
bool MainMenuScene::init()
{
	if (!Layer::init())
	{
		return false;
	}
	
	auto screenSize = Director::getInstance()->getVisibleSize();

	//quit board
	auto quitboard = Sprite::create("ui/quitboard.png");
	quitboard->setPosition(screenSize / 2);
	quitboard->setVisible(false);
	addChild(quitboard, 2);

	auto acceptButton = ui::Button::create("ui/agreenormal.png", "ui/agreepress.png");
	acceptButton->setPosition(Vec2(113.0f, 32.0f));
	acceptButton->addClickEventListener([&](Ref* render)
	{
		Director::getInstance()->end();
		#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		exit(0);
		#endif
	});
	quitboard->addChild(acceptButton);

	auto cancelButton = ui::Button::create("ui/cancelnormal.png", "ui/cancelpress.png");
	cancelButton->setPosition(Vec2(277.0f, 30.0f));
	cancelButton->addClickEventListener([=](Ref* render)
	{
		quitboard->setVisible(false);
	});
	quitboard->addChild(cancelButton);

	//background
	auto logo = Sprite::create("backgrounds/backgourdmenu.jpg");
	logo->setPosition(screenSize.width / 2, screenSize.height / 2);
	logo->setScale(SCENE_W / logo->getContentSize().width, SCENE_H / logo->getContentSize().height);
	addChild(logo);

	
	if (GameSetting::getInstance()->isEnableMusic())
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
		CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("sounds/BGmusic.mp3");
	}

	//rain
	auto animation = ParticleSystemQuad::create("particles/mua.plist");
	animation->setPosition(screenSize.width / 2, screenSize.height / 2);
	addChild(animation, 3);
	

	//btmenu
	auto ItemPlay = MenuItemImage::create("ui/btplay.png", "ui/btplay2.png", [](Ref* sender) {

		if (!GameSetting::getInstance()->getIsShowTrailer())
		{
			auto Intro = Intro::createScene();
			Director::getInstance()->replaceScene(Intro);
		}
		else
		{
			auto waitScene = WaitScene::createScene();
			Director::getInstance()->replaceScene(waitScene);
		}

	});
	ItemPlay->setAnchorPoint(Vec2(0.0f,0.0f));
	ItemPlay->setPosition(115, screenSize.height*0.75f);
	ItemPlay->setScale(0.5);
	
	auto ItemShop = MenuItemImage::create("ui/shop1.png", "ui/shop2.png", [](Ref* sender) {
		GameSetting::getInstance()->back = 0;
		auto MenuScence = MainMenuScene::createScene();
		Director::getInstance()->replaceScene(TransitionFadeTR::create(2.0f, ShopItem::createScene()));

	});
	ItemShop->setAnchorPoint(Vec2(0.0f, 0.0f));
	ItemShop->setPosition(115, screenSize.height*0.6);
	ItemShop->setScale(0.5);

	auto ItemAbout = MenuItemImage::create("ui/about.png", "ui/about2.png", [](Ref* sender)
	{
		auto MenuScence = AboutScene::createScene();

		Director::getInstance()->replaceScene(TransitionFadeTR::create(2.0f, AboutScene::createScene()));

	});
	ItemAbout->setAnchorPoint(Vec2(0.0f, 0.0f));
	ItemAbout->setPosition(115, screenSize.height*0.45f);
	ItemAbout->setScale(0.5);

	auto ItemQuit = MenuItemImage::create("ui/btQuit.png", "ui/btQuit2.png", [=](Ref* sender) {
		quitboard->setVisible(true);

	}); 
	ItemQuit->setAnchorPoint(Vec2(0.0f, 0.0f));
	ItemQuit->setPosition(115, screenSize.height*0.3f);
	ItemQuit->setScale(0.5);
	//setting
	auto ItemSetting = MenuItemImage::create("ui/setting.png", "ui/setting2.png", [](Ref* sender) {
		auto MenuScence = SettingScene::createScene();

		Director::getInstance()->replaceScene(MenuScence);

	});
	ItemSetting->setPosition(750, 440);
	ItemSetting->setScale(0.4);


	//help

	auto ItemHelp = MenuItemImage::create("ui/help.png", "ui/help2.png", [](Ref* sender) {
		auto MenuScence = HelpScene::createScene();

		Director::getInstance()->replaceScene(MenuScence);

	});
	ItemHelp->setPosition(680, 440);
	ItemHelp->setScale(0.4);

	auto myMenu = Menu::create(ItemPlay, ItemShop /*ItemHigh*/,ItemAbout,ItemQuit,ItemSetting,ItemHelp,nullptr);
	myMenu->setPosition(0, 0);
	addChild(myMenu);

	return true;
}