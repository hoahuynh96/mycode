#include "Model\Background.h"

USING_NS_CC;

Background::Background(cocos2d::Layer* layer, char str[50])
{
	background1 = Sprite::createWithSpriteFrameName(str);
	background1->setAnchorPoint(Vec2(0.0f, 0.0f));
	background1->setPosition(Vec2(0.0f, 0.0f));

	layer->addChild(background1,-5);

	background2 = Sprite::createWithSpriteFrameName(str);
	background2->setAnchorPoint(Vec2(0.0f, 0.0f));
	background2->setPosition(Vec2(background2->getContentSize().width, 0.0f));

	layer->addChild(background2, -5);
}

Background::~Background()
{

}

void Background::scrollLeft()
{
	background1->setPositionX(background1->getPositionX() + 1.0f);
	background2->setPositionX(background2->getPositionX() + 1.0f);

	if (background1->getPosition().x >= background1->getContentSize().width)
	{
		background1->setPositionX(background2->getPositionX()-background2->getContentSize().width);
	}

	if (background2->getPosition().x >= background2->getContentSize().width+10.0f)
	{
		background2->setPositionX(background1->getPositionX()-background1->getContentSize().width);
	}
}

void Background::scrollRight()
{
	background1->setPositionX(background1->getPositionX() - 1.0f);
	background2->setPositionX(background2->getPositionX() - 1.0f);

	if (background1->getPosition().x <= -background1->getContentSize().width)
	{
		background1->setPositionX(background2->getPositionX() + background2->getContentSize().width);
	}

	if (background2->getPosition().x <= -background2->getContentSize().width)
	{
		background2->setPositionX(background1->getPositionX()+background1->getContentSize().width);
	}
}