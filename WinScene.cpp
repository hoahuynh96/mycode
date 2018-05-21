#include "GUI\WinScene.h"
#include "ui\CocosGUI.h"
#include "Model\GameSetting.h"
#include "GUI\PlayScene.h"

#include "Define.h"

USING_NS_CC;

Scene* WinScene::createScene()
{
	auto scene = Scene::create();
	auto layer = WinScene::create();
	scene->addChild(layer);
	return scene;
}

bool WinScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	//background
	
	
	auto screenSize = Director::getInstance()->getVisibleSize();
	auto back = Sprite::create("backgrounds/victory.png");
	//back->setScale(0.3f);
	back->setPosition(screenSize.width / 2, screenSize.height / 2 );
	addChild(back);

	auto explode = ParticleSystemQuad::create("particles/win.plist");
	explode->setPosition(Vec2(screenSize.width / 2, screenSize.height / 2));

	addChild(explode);

	auto visibleSize = Director::getInstance()->getVisibleSize();
	//next button
	auto nextButton = ui::Button::create("ui/continue.png");
	nextButton->setPosition(Vec2(300,100));
	nextButton->setAnchorPoint(Vec2(0.0f, 0.5f));
	//nextButton->setScale(0.5f);
	addChild(nextButton);
	nextButton->addClickEventListener([=](Ref* render){
		auto montersScene = PlayScene::createScene();
		Director::getInstance()->replaceScene(montersScene);
	});

	return true;
}
