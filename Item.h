#ifndef _ITEM_H_
#define _ITEM_H_
#include "cocos2d.h"

class Item
{
public:
	Item(cocos2d::TMXTiledMap* tileMap, char nameSprite[50], int type);
	
	~Item();

	int getValue(); 

	void setValue(int valueItem);

	void setPosition(float x, float y);

	void setIsDislay(bool isDislay);

	bool getIsDislay();

	void setGroupIndex(int index);

	int getTypeItem();

	void monneyAnimation(cocos2d::Action* action);
private:

	int valueItem;

	cocos2d::Sprite* mSprite;

	cocos2d::PhysicsBody* mPhysicsBody;

	bool isDislay;

	int typeItem;
};

#endif