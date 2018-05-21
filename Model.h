#ifndef _MODEL_H_
#define _MODEL_H_

#include "ui\CocosGUI.h"
#include "cocos2d.h"

class Model
{

protected:
	float totalBlood;

	float currentBlood;

	cocos2d::Sprite* mSprite;

	cocos2d::PhysicsBody* mPhysicsBody;

	bool mIsDislay;

	int directionRun;  // xem huong quoay cua nhan vat

	bool isMove; // kiem tra nhan vat co di chuyen hay khong

	cocos2d::ui::LoadingBar* bloodBar;

	cocos2d::Sprite* bloodBarBG;

	cocos2d::Sprite* bloodBarBG1;

	bool isAlive;

	cocos2d::Node* scopeDamage;

	cocos2d::PhysicsBody* scopeDamageBody;

	int damage;

	bool isKeepKey;
public:
	Model();

	~Model();

	void setIsDislay(bool alive);

	bool getIsDislay();

	void setPosition(cocos2d::Vec2 pos);

	void setAnchorPoint(cocos2d::Vec2 anchor);

	void setScale(float scale);

	cocos2d::Vec2 getPosition();

	cocos2d::Size getContentSize();

	cocos2d::Animate* getAnimate(const char* format, int numOfImage, float timeDelay);

	void setPositionX(float x);

	float getPositionX();

	void setPositionY(float y);

	float getPositionY();

	void turnLeft();

	void turnRight();

	void setDirection(int direction);

	int getDirection();

	void setIsMove(bool isMove);

	bool getIsMove();

	int getNumberOfRunningActions();

	cocos2d::Rect getBoundingBox();

	void setTotalBlood(float totalBlood);

	float getTotalBlood();

	void setCurrentBlood(float currentBlood);

	float getCurrentBlood();

	bool getIsAlive();

	void setIsAlive(bool isAlive);

	int getDamage();

	void setKeepKey(bool isKeepKey);

	int getKeepKey();
};
#endif