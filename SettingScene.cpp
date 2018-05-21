#include "GUI\MainMenuScene.h"
#include "GUI\SettingScene.h"
#include "Model\GameSetting.h"

#include"Define.h"

USING_NS_CC;

Scene* SettingScene::createScene(){
	auto scene = Scene::create();
	auto layer = SettingScene::create();
	scene->addChild(layer);
	return scene;
}
bool SettingScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();

	//background
	auto screenSize = Director::getInstance()->getVisibleSize();
	auto logo = Sprite::create("backgrounds/backgourdmenu.jpg");
	logo->setPosition(screenSize.width / 2, screenSize.height / 2);
	logo->setScale(SCENE_W / logo->getContentSize().width, SCENE_H / logo->getContentSize().height);
	addChild(logo);


	//rain
	auto animation = ParticleSystemQuad::create("particles/mua.plist");
	animation->setPosition(screenSize.width / 2, screenSize.height / 2);
	addChild(animation, 3);

	//board
	auto screenSize2 = Director::getInstance()->getVisibleSize();
	auto board = Sprite::create("ui/board.png");

	board->setAnchorPoint(Vec2(0.0f, 0.0f));
	board->setPosition(70, 50);
	addChild(board, 2);

	//music

	auto music = Sprite::create("ui/Music.png");

	music->setAnchorPoint(Vec2(0.0f, 0.0f));
	music->setScale(0.5f);
	music->setPosition(115, 340);
	addChild(music, 2);

	//sound

	auto sound = Sprite::create("ui/sound1.png");

	sound->setAnchorPoint(Vec2(0.0f, 0.0f));
	sound->setScale(0.5f);
	sound->setPosition(285, 340);
	addChild(sound, 2);

	//sound

	auto volum = Sprite::create("ui/volume.png");

	volum->setAnchorPoint(Vec2(0.0f, 0.0f));
	volum->setScale(0.5f);
	volum->setPosition(235, 200);
	addChild(volum, 2);

	buttonmusic = ui::CheckBox::create("ui/off.png", "ui/on.png");
	buttonmusic->setPosition(Vec2(170, 280));
	buttonmusic->setScale(0.7f);
	buttonmusic->setSelected(GameSetting::getInstance()->isEnableMusic());
	buttonmusic->addEventListener([&](Ref* sender, ui::CheckBox::EventType type)
	{

		switch (type)
		{
		case ui::CheckBox::EventType::SELECTED:
			GameSetting::getInstance()->setEnableMusic(1);
			CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
			CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("sounds/BGmusic.mp3", true);
			break;
		case ui::CheckBox::EventType::UNSELECTED:
			GameSetting::getInstance()->setEnableMusic(0);
			CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
			break;
		default:
			break;
		}

		if (!buttonmusic->isSelected() && !buttonSound->isSelected())
		{
			slider->setEnabled(false);
		}
		else
		{
			slider->setEnabled(true);
		}
	});
	
	addChild(buttonmusic);

	buttonSound = ui::CheckBox::create("ui/off.png", "ui/on.png");
	buttonSound->setPosition(Vec2(360, 280));
	buttonSound->setScale(0.7f);
	buttonSound->setSelected(GameSetting::getInstance()->isEnableSound());
	buttonSound->addEventListener([&](Ref* sender, ui::CheckBox::EventType type){
		switch (type)
		{
		case ui::CheckBox::EventType::SELECTED:
			GameSetting::getInstance()->setEnableSound(1);
			break;
		case ui::CheckBox::EventType::UNSELECTED:
			GameSetting::getInstance()->setEnableSound(0);
			break;
		default:
			break;
		}

		if (!buttonmusic->isSelected() && !buttonSound->isSelected())
		{
			slider->setEnabled(false);
		}
		else
		{
			slider->setEnabled(true);
		}
	});
	addChild(buttonSound);
	
	slider = ui::Slider::create();
	slider->loadBarTexture("ui/slider_bar_bg.png");
	slider->loadSlidBallTextures("ui/butonslide-nomal.png", "ui/butonslide.png");
	slider->loadProgressBarTexture("ui/slidebar.png");
	slider->setScale(0.7f);
	slider->setPosition(Vec2(275, 170));
	slider->setPercent(GameSetting::getInstance()->getVolume());
	slider->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type)
	{
		switch (type)
		{
		case ui::Widget::TouchEventType::ENDED:
			GameSetting::getInstance()->setVolume(slider->getPercent());
			CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(GameSetting::getInstance()->getVolume()/100.0f);
			CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(GameSetting::getInstance()->getVolume()/100.0f);
			break;
		}
	});

	if (!buttonmusic->isSelected() && !buttonSound->isSelected())
	{
		slider->setEnabled(false);
	}

	addChild(slider, 2);
	//back
	auto back = ui::Button::create("ui/back.png", "ui/back_p.png");

	back->setPosition(Vec2(280, 110));
	back->setScale(0.2f);
	back->addClickEventListener([&](Ref* event) {

		GameSetting::getInstance()->saveData();
		Director::getInstance()->replaceScene(TransitionFlipAngular::create(0.5f, MainMenuScene::createScene()));
	});
	addChild(back, 2); 

	return true;
}
