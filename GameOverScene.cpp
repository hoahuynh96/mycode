#include "GUI\GameOverScene.h"
#include "ui\CocosGUI.h"
#include "GUI\WaitScene.h"
#include "GUI\PlayScene.h"

#include "Define.h"

USING_NS_CC;

Scene* GameOverScene::createScene()
{
	auto scene = Scene::create();
	auto layer = GameOverScene::create();
	scene->addChild(layer);
	return scene;
}

bool GameOverScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	//background
	auto screenSize = Director::getInstance()->getVisibleSize();
	auto back = Sprite::create("backgrounds/gameover.png");
	back->setPosition(screenSize.width / 2, screenSize.height / 2);
	back->setScale(SCENE_W / back->getContentSize().width, SCENE_H / back->getContentSize().height);
	addChild(back);


	auto visibleSize = Director::getInstance()->getVisibleSize();

	auto boardMenu = Sprite::create("ui/board.png");
	boardMenu->setPosition(visibleSize / 2);
	boardMenu->setContentSize(Size(300.0f, 120.0f));
	addChild(boardMenu, 10);

	//home button
	auto homeButton = ui::Button::create("ui/homenormal.png", "ui/homepress.png");
	homeButton->loadTextureNormal("homenormal.png", ui::Widget::TextureResType::PLIST);
	homeButton->loadTexturePressed("homepress.png", ui::Widget::TextureResType::PLIST);
	homeButton->setPosition(Vec2(boardMenu->getContentSize().width / 2 - 20, boardMenu->getContentSize().height / 2));
	homeButton->setAnchorPoint(Vec2(1.0f, 0.5f));
	homeButton->setScale(0.5f);
	boardMenu->addChild(homeButton);
	homeButton->addClickEventListener([](Ref* render){
		auto waitScene = WaitScene::createScene();
		Director::getInstance()->replaceScene(waitScene);
	});

	//replay button
	auto replayButton = ui::Button::create("ui/replaynormal.png", "ui/replaypress.png");
	replayButton->loadTextureNormal("replaynormal.png", ui::Widget::TextureResType::PLIST);
	replayButton->loadTexturePressed("replaypress.png", ui::Widget::TextureResType::PLIST);
	replayButton->setPosition(Vec2(boardMenu->getContentSize().width / 2 + 20, boardMenu->getContentSize().height / 2));
	replayButton->setAnchorPoint(Vec2(0.0f, 0.5f));
	replayButton->setScale(0.5f);
	boardMenu->addChild(replayButton);
	replayButton->addClickEventListener([=](Ref* render){
		auto playScene = PlayScene::createScene();
		Director::getInstance()->replaceScene(playScene);
	});

	return true;
}
