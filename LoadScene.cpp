#include "GUI\LoadScene.h"
#include "GUI\MainMenuScene.h"
#include "Model\GameSetting.h"
#include "GUI\MonsterSecen.h"
#include "Define.h"

USING_NS_CC;

extern int gCount;

Scene* LoadScene::createScene()
{
	auto scene = Scene::create();

	scene->addChild(LoadScene::create());

	return scene;
}
bool LoadScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	gCount = 0;

	auto visibleSize = Director::getInstance()->getVisibleSize();

	CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(GameSetting::getInstance()->getVolume() / 100.0f);
	CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(GameSetting::getInstance()->getVolume() / 100.0f);

	//backgound
	auto logo = Sprite::create("backgrounds/loadsceneBG.png");
	logo->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	logo->setScale(visibleSize.width / logo->getContentSize().width, visibleSize.height / logo->getContentSize().height);
	logo->setOpacity(0);
	addChild(logo,-1);
	
	auto logoFadein = FadeIn::create(2);
	logo->runAction(logoFadein);

	//logo
	auto screenSize1 = Director::getInstance()->getVisibleSize();
	auto namegame = Sprite::create("namegame.png");
	namegame->setPosition(500, 250);
	namegame->setScale(0.3f);
	addChild(namegame);
	
	auto ScaleTo = ScaleTo::create(2, 0.6f);
	auto Sequence = Sequence::create(ScaleTo, nullptr);
	namegame->runAction(Sequence);

	//Loading background
	loadingBG = Sprite::create("ui/loadingBG.png");
	loadingBG->setPosition(visibleSize.width/2,60 );
	loadingBG->setVisible(false);
	addChild(loadingBG,1);

	//Loading bar
	loadingBar = ui::LoadingBar::create("ui/loadingbar.png",0.0f);
	loadingBar->setDirection(ui::LoadingBar::Direction::LEFT);
	loadingBar->setPosition(loadingBG->getContentSize() / 2);
	loadingBG->addChild(loadingBar,2);

	scheduleUpdate();

	return true;
}
void LoadScene::update(float dt)
{
	gCount++;

	//sau 2s bat dau hien thi loading bar
	if (gCount == 2*FPS )
	{
		loadingBG->setVisible(true);
		loadingBar->setVisible(true);
	}

	//Tang loaddingBar len 100% tu giay thu 2, den giay thu 4
	if (gCount > 2 * FPS&&gCount <= 4 * FPS)
	{
		loadingBar->setPercent((gCount - 2 * FPS) * 100 / (2 * FPS));
		if (gCount == 2 * FPS + 2)
		{
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile("monters/dragon.plist");
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Arrow/arrow.plist");
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile("backgrounds/background.plist");
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile("items/itemgame.plist");
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile("items/itemgame2.plist");
			//GameSetting::getInstance()->saveData();
			GameSetting::getInstance()->loadData(); 
			GameSetting::getInstance()->preLoadSounds();
			
		}
		else if (gCount == 2 * FPS + 30)
		{

			SpriteFrameCache::getInstance()->addSpriteFramesWithFile("player/player1.plist");
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile("player/player2.plist");
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile("player/playerskill.plist");
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile("player/bloodPlayer.plist");
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile("monters/knight.plist");
		}
		else if (gCount == 3 * FPS)
		{
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/skill.plist");
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/runback.plist");
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/da.plist");
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/acttack.plist");
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/runBoss.plist");
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile("monters/assassin.plist");
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile("monters/Mercenary.plist");
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile("items/moneysprite.plist");
		}
		else if (gCount == 3 * FPS + 20)
		{
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile("monters/crepattack.plist");
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile("monters/creprun.plist");
			//SpriteFrameCache::getInstance()->addSpriteFramesWithFile("monters/fire.plist");
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boss/fire.plist");
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile("monters/boss1.plist");
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile("ui/gate1.plist");
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile("ui/controlbutton.plist");
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile("weapons/weapon.plist");
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile("ui/gui2.plist");
		}
		
	}

	//Sau 4s thi chuyen den MainMenuScene
	if (gCount > 4 * FPS)
	{
		auto scene = MainMenuScene::createScene();
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile("ui/controlbutton.plist");
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile("weapons/weapon.plist");
	}

	//Sau 4s thi chuyen den MainMenuScene
	if (gCount > 4 * FPS)
	{
		auto scene = MainMenuScene::createScene();
		//auto scene = MonsterSecen::createScene();
		Director::getInstance()->replaceScene(scene);
	}
}