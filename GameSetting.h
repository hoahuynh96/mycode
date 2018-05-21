#ifndef _GAME_SETTING_H_
#define _GAME_SETTING_H_

#include "SimpleAudioEngine.h"

class GameSetting
{
private:
	static GameSetting* sInstance;

	int settingValue[12];

	int level;

	GameSetting();

public:
	int back;

	~GameSetting();

	static GameSetting* getInstance();

	void setEnableMusic(int isEnableMusic);

	bool isEnableMusic();

	void setEnableSound(int isEnableSound);

	bool isEnableSound();

	void setVolume(int volume);

	int getVolume();

	int getMoney();

	void setMoney(int money);

	void loadData();

	void saveData();

	int getLevel();

	void setLevel(int level);

	int getNumberKunai();

	void setNumberKunai(int numberKunai);

	int getBloodSpell();

	void setBloodSpell(int numberSpell);

	int getArmor();

	void setArmor(int armor);

	int getPlayerLevel();

	void setPlayerLevel(int playerLevel);

	int getEXP();

	void setEXP(int exp);

	int getBloodPlayer();

	void setBloodPlayer(int blood);

	int getDamgePlayer();

	void setDamePlayer(int damage);

	void setIsShowTrailer(int isShow);

	bool getIsShowTrailer();

	void preLoadSounds();
};

#endif