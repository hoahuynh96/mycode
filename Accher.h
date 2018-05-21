#ifndef _TESTARCHERS_H_
#define _TESTARCHERS_H_
#include "Model\Model.h"
#include "Model\Arrow.h"

#include "cocos2d.h"


class Accher : public Model
{
private:

	Arrow* arrow;

public:

	bool Shooting(float xTarget, float yTarget);

	Accher(cocos2d::TMXTiledMap* layer);

	~Accher();

	cocos2d::Action* actionOfMonser[1];

	void update();

	void hasCollision();

	void setIndex(int index);
};

#endif