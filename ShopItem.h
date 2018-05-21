#ifndef _SHOPITEM_
#define _SHOPTIEM_

#include "Model\GameSetting.h"
#include "cocos2d.h"

class ShopItem : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

private:
	int price;

	int number;

	int type;

	GameSetting* gameSetting;

	cocos2d::Sprite* mSprite;

	cocos2d::Sprite* mSprite1;

	cocos2d::Sprite* spriteCharacter;

	cocos2d::Sprite* spritebg;

	cocos2d::Label* numberItem;

	cocos2d::Label* money;

	cocos2d::Label* totalMoney;

	cocos2d::Label* numberSpell;

	cocos2d::Label* numberKunai;

	virtual bool init();

	void createItemButton();

	void createBoardShop();

	void dislayBoardShop();

	void setLabel();

	CREATE_FUNC(ShopItem);
};

#endif