#ifndef _ASSASSIN_H_
#define _ASSASSIN_H_
#include "Model\Model.h"
#include "cocos2d.h"

class Assassin : public Model
{
public:
	Assassin(cocos2d::TMXTiledMap* layer);

	~Assassin();

	void Attack();

	void Idel();

	void Update();

	void setIndex(int index);

	void setMinPosition(int minPosition);

	int getMinPosition();

	void setMaxPosition(int maxPosition);

	int getMaxPosition();

	void autoWalk();

	void chase(float x);
private:
	void Damage();

	cocos2d::Action* listAction[2];

	int currentAction;

	int minPosition;

	int maxPosition;
};

#endif