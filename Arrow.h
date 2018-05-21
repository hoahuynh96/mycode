#ifndef _TESTARROW_H_
#define _TESTARROW_H_

#include "Model\Weapon.h"
#include "cocos2d.h"

class Arrow : public Weapon
{


public:
	
	Arrow(cocos2d::TMXTiledMap* layer);
	~Arrow();
	void setRowtation(float angle);
};


#endif