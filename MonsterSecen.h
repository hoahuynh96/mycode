#ifndef _MONSTER_SECEN_H
#define _MONSTER_SECEN_H

#include "Model\Assassin.h"
#include "Model\Player.h"
#include "Model\Accher.h"
#include "ui\CocosGUI.h"
#include "Model\GameSetting.h"
#include "Model\MosterBoss.h"
#include "Model\Dragon.h"
#include "Model\Ultimate.h"
#include "Model\Crep.h"
#include "Model\PowerSword.h"
#include <vector>
#include "cocos2d.h"

using namespace std;
class MonsterSecen : public cocos2d::Layer
{
private:
	int check = 0;

	int check1 = 0;
	
	cocos2d::TMXTiledMap* physicsTileMap;

	Ultimatel* ulti;

	Player* mainPlayer;

	Player* play;

	MosterBoss* mosterBoss;

	Dragon* dragon;

	PowerSword* powerSword;

	Ultimatel* ultimatel;

	Crep* crep;

	//Monster

	GameSetting* gameSetting;

	cocos2d::Sprite* moveLeftPress;

	cocos2d::Sprite* moveLeftNormal;

	cocos2d::Sprite* moveRightPress;

	cocos2d::Sprite* moveRightNormal;

	cocos2d::Size visibleSize;

	cocos2d::Label* money;

	cocos2d::Label* numberKunaiLabel;

	cocos2d::Label* bloodSpellLabel;

	cocos2d::ui::Button* kunaiButton;

	cocos2d::ui::Button* spellButton;

	cocos2d::ui::Button* untyButton;

	cocos2d::Sprite* powerBarBG;

	cocos2d::ui::LoadingBar* powerBar;

	void moveLeft();

	void moveRight();

	void createUi();

	void createButton();

	/*void createEnemy();*/

	void createPhysic();

	//void createItem();

	void updateEnemy();

	void updateMoney(int money);

	void updateKunai();

	void updateBloodSpell();

	void updatePowerBar();

public:
	vector<Ultimatel*> ultiList;

	cocos2d::ParticleSystemQuad* bloodparticle;

	static cocos2d::Scene* createScene();

	virtual bool init();

	virtual void update(float dt);

	virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);

	virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event);

	virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event);

	void setCenterView();

	bool onContactBegin(cocos2d::PhysicsContact& contact);

	CREATE_FUNC(MonsterSecen);
};

#endif