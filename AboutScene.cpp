#include "GUI\AboutScene.h"
#include "GUI\MainMenuScene.h"
#include <ui/CocosGUI.h>
#include "Define.h"

USING_NS_CC;

Scene* AboutScene::createScene()
{
	auto scene = Scene::create();
	auto layer = AboutScene::create();
	scene->addChild(layer);
	return scene;
}
bool AboutScene::init()
{
	if (!Layer::init())
	{
		return false;
	}
	
	auto logo = Sprite::create("backgrounds/BackgroundScroll.jpg");
	logo->setContentSize(Director::getInstance()->getVisibleSize());
	logo->setPosition(Director::getInstance()->getVisibleSize() / 2);
	addChild(logo);

	auto backButton = ui::Button::create();
	backButton->loadTextureNormal("backnormal.png", ui::Widget::TextureResType::PLIST);
	backButton->loadTexturePressed("backpress.png", ui::Widget::TextureResType::PLIST);
	backButton->addClickEventListener([&](Ref* event)
	{
		Director::getInstance()->replaceScene(TransitionFadeTR::create(2.0f, MainMenuScene::createScene()));
	});
	backButton->setAnchorPoint(Vec2(0.1f, 0.1f));
	backButton->setPosition(Vec2(10.0f, 10.0f));
	backButton->setScale(0.5);
	addChild(backButton, 2);

	return true;
}
