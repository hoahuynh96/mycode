#include "Model\GameSetting.h"
#include "cocos2d.h"
#include <fstream>

#include "Define.h"

USING_NS_CC;

GameSetting* GameSetting::sInstance = nullptr;

GameSetting::GameSetting()
{
	settingValue[MONEY] = 5000;
	settingValue[VOLUME] = 100;
	settingValue[ENABLEMUSIC] = 1;
	settingValue[ENABLESOUND] = 1;
	settingValue[NUMBER_KUNAI] = 5;
	settingValue[BLOOD_SPELL] = 0;
	settingValue[ARMOR] = 10;
	settingValue[PLAYER_LEVEL] = 1;
	settingValue[EXP] = 0;
	settingValue[DAMAGE_PLAYER] = 10;
	settingValue[BLOOD_PLAYER] = 300;
	settingValue[SHOW_TRAILER] = 0;

	level = 1;
}

GameSetting::~GameSetting()
{

}

GameSetting* GameSetting::getInstance()
{
	if (sInstance == nullptr)
	{
		sInstance = new GameSetting();
	}
	return sInstance;
}

void GameSetting::setEnableMusic(int isEnableMusic)
{
	settingValue[ENABLEMUSIC] = isEnableMusic;
}

bool GameSetting::isEnableMusic()
{
	return settingValue[ENABLEMUSIC] == 1;
}

void GameSetting::setEnableSound(int isEnableSound)
{
	settingValue[ENABLESOUND] = isEnableSound;
}

bool GameSetting::isEnableSound()
{
	return settingValue[ENABLESOUND] == 1;
}

void GameSetting::setVolume(int volume)
{
	settingValue[VOLUME] = volume;
}

int GameSetting::getVolume()
{
	return settingValue[VOLUME];
}

int GameSetting::getMoney()
{
	return settingValue[MONEY];
}

void GameSetting::setMoney(int money)
{
	settingValue[MONEY] = money;
}

void GameSetting::loadData()
{
	std::string filePath = FileUtils::getInstance()->getWritablePath() + "data.txt";

	std::ifstream readFile;
	readFile.open(filePath.c_str());

	int index = 0;
	char str[50];
	
	//doc file theo thu tu
	//1. money
	//2. mVolume
	//3. isEnableMusic
	//4. isEnableSound
	//5. number_Kunai
	
	while (readFile>>str)
	{
		settingValue[index++] = atoi(str);
	}

	readFile.close();
}

void GameSetting::saveData()
{
	std::string filePath = FileUtils::getInstance()->getWritablePath() + "data.txt";

	std::ofstream writeFile;

	writeFile.open(filePath.c_str());

	for (int i = 0; i < 12; i++)
	{
		writeFile << settingValue[i] << " ";
	}

	writeFile.close();
}

int GameSetting::getLevel()
{
	return level;
}

void GameSetting::setLevel(int level)
{
	this->level = level;
}


int GameSetting::getNumberKunai()
{
	return settingValue[NUMBER_KUNAI];
}

void GameSetting::setNumberKunai(int numberKunai)
{
	settingValue[NUMBER_KUNAI] = numberKunai;
}

int GameSetting::getBloodSpell()
{
	return settingValue[BLOOD_SPELL];
}

void GameSetting::setBloodSpell(int numberSpell)
{
	settingValue[BLOOD_SPELL] = numberSpell;
}

int GameSetting::getArmor()
{
	return settingValue[ARMOR];
}

void GameSetting::setArmor(int armor)
{
	settingValue[ARMOR] = armor;
}

int GameSetting::getPlayerLevel()
{
	return settingValue[PLAYER_LEVEL];
}

void GameSetting::setPlayerLevel(int playerLevel)
{
	settingValue[PLAYER_LEVEL] = playerLevel;
}

int GameSetting::getEXP()
{
	return settingValue[EXP];
}

void GameSetting::setEXP(int exp)
{
	settingValue[EXP] = exp;
}

int GameSetting::getBloodPlayer()
{
	return settingValue[BLOOD_PLAYER];
}

void GameSetting::setBloodPlayer(int blood)
{
	settingValue[BLOOD_PLAYER] = blood;
}

int GameSetting::getDamgePlayer()
{
	return settingValue[DAMAGE_PLAYER];
}

void GameSetting::setDamePlayer(int damage)
{
	settingValue[DAMAGE_PLAYER] = damage;
}

void GameSetting::setIsShowTrailer(int isShow)
{
	settingValue[SHOW_TRAILER] = isShow;
}

bool GameSetting::getIsShowTrailer()
{
	return settingValue[SHOW_TRAILER] == 1 ? true : false;
}

void GameSetting::preLoadSounds()
{
	auto audio = CocosDenshion::SimpleAudioEngine::getInstance();

	audio->preloadEffect("sounds/BanCung.mp3");
	audio->preloadEffect("sounds/BiDanhTrung.mp3");
	audio->preloadEffect("sounds/BigBang.mp3");
	audio->preloadEffect("sounds/Chem.wav");
	audio->preloadEffect("sounds/GameOver.mp3");
	audio->preloadEffect("sounds/JumpPlayer.mp3");
	audio->preloadEffect("sounds/LuomTien.wav");
	audio->preloadEffect("sounds/PhongPhiTieu.mp3");
	audio->preloadBackgroundMusic("sounds/BGmusic.mp3");
	
}
