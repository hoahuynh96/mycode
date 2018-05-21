#ifndef _PLAYER_H_
#define _PLAYER_H_
#include "Model\Model.h"
#include "Model\Kunai.h"
#include "Model\PowerSword.h"
#include "cocos2d.h"

#include "Define.h"

class Player : public Model
{
public:
	Player(cocos2d::Layer* layer);

	~Player();

	void Idle();

	void Jump();

	void Run();

	void Attack();

	void Slide();

	void ThrowKunai();

	int getCurrentAction();

	void update();

	void hasConclusion(int index);

	void unty();

	bool getPreventLeft();

	void setPreventLeft(bool preventLeft);

	bool getPreventRight();

	void setPreventRight(bool preventRight);

	void beingAttacked(int damage);

	void upLevel();

	void setIsMove(bool isMove);

	bool getIsMove();

	void setIsOnland(bool isOnland);

	bool getIsOnland();
private:
	void setKunai();

	void Damage();

	bool isOnLand;

	std::vector<Kunai*> kunais;

	//cocos2d::Sprite* sword;

	cocos2d::Action* actionOfPlayer[NUMBERACTION];

	int currentAction;

	PowerSword* powerSword;

	bool preventLeft;

	bool preventRight;

	int armor;

	bool isMove; // kiem tra nhan vat co di chuyen hay khong
};

#endif