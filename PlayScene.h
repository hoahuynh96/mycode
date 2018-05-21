#ifndef _PLAY_SCENE_H_
#define _PLAY_SCENE_H_

#include "Model\Assassin.h"
#include "Model\Player.h"
#include "Model\Accher.h"
#include "Model\Background.h"
#include "ui\CocosGUI.h"
#include "Model\GameSetting.h"
#include "Model\Item.h"
#include "Model\Mercenary.h"
#include "Model\Knight.h"
#include "cocos2d.h"


class PlayScene : public cocos2d::Layer
{
private:
	Background* backGround;

	cocos2d::TMXTiledMap* physicsTileMap;

	Player* mainPlayer;

	GameSetting* gameSetting;

	std::vector<Assassin*> assassins;

	std::vector<Accher*> acchers;

	std::vector<Item*> items;

	std::vector<Mercenary*> mercenarys;

	std::vector<Knight*> knights;

	cocos2d::Sprite* moveLeftPress;

	cocos2d::Sprite* moveLeftNormal;

	cocos2d::Sprite* moveRightPress;

	cocos2d::Sprite* moveRightNormal;

	cocos2d::Sprite* gate;

	//cocos2d::Sprite* keyItem;

	cocos2d::Size visibleSize;

	cocos2d::Label* money;

	cocos2d::Label* numberKunaiLabel;

	cocos2d::Label* bloodSpellLabel;

	cocos2d::Label* expLabel;

	cocos2d::Label* levelLabel;

	cocos2d::Label* keyLabel;

	cocos2d::ui::Button* kunaiButton;

	cocos2d::ui::Button* spellButton;

	cocos2d::ui::Button* untyButton;

	cocos2d::ui::LoadingBar* powerBar;

	cocos2d::ui::LoadingBar* expBar;

	cocos2d::Action* moneyAnimate;

	void moveLeft();

	void moveRight();

	void createUi();

	void createButton();

	void createEnemy();

	void createPhysic();

	void createItem();

	void updateEnemy();

	void updateMoney(int money);

	void updateKunai();

	void updateBloodSpell();

	void updatePowerBar(int percent);

	void showItem(float x, float y, int type);

	void playerCollectItem(int index);

	void playerCollectExp(int exp, cocos2d::Vec2 position);

	void openGate();

public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	virtual void update(float dt);

	virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);

	virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event);

	virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event);

	void setCenterView();

	virtual bool onContactBegin(cocos2d::PhysicsContact& contact);

	virtual void onContactSeparate(cocos2d::PhysicsContact& contact);

	virtual bool onContactPreSolve(cocos2d::PhysicsContact& contact);

	CREATE_FUNC(PlayScene);
};

#endif