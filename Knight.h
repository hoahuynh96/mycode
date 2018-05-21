#ifndef _KNIGHT_H_
#define _KNIGHT_H_

#include "Model\Model.h"
#include "cocos2d.h"

class Knight : public Model
{
public:
	Knight(cocos2d::TMXTiledMap* layer);

	~Knight();

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