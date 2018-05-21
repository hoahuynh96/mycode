#ifndef _BACKGROUND_H_
#define _BACKGROUND_H_

#include "cocos2d.h"

class Background
{
private:
	cocos2d::Sprite* background1;

	cocos2d::Sprite* background2;

public:

	Background(cocos2d::Layer* layer, char str[50]);

	~Background();

	void scrollLeft();

	void scrollRight();
};

#endif