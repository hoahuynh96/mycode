#ifndef _SETTING_SCENE_H_
#define _SETTING_SCENE_H_

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class SettingScene : public cocos2d::Layer
{
private:
	cocos2d::ui::CheckBox* buttonmusic;

	cocos2d::ui::CheckBox* buttonSound;

	cocos2d::ui::Slider* slider;

public:

	static cocos2d::Scene* createScene();

	virtual bool init();

	CREATE_FUNC(SettingScene);
};

#endif 