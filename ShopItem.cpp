#include "GUI\ShopItem.h"
#include "GUI\MainMenuScene.h"
#include "GUI\WaitScene.h"
#include "ui/CocosGUI.h"

#include "Define.h"
USING_NS_CC;

Scene* ShopItem::createScene()
{
	auto scene = Scene::create();
	auto layer = ShopItem::create();
	scene->addChild(layer);
	return scene;
}

bool ShopItem::init()
{
	if (!Layer::init())
	{
		return false;
	}

	gameSetting = GameSetting::getInstance();
	number = 0;
	//background
	auto screenSize = Director::getInstance()->getVisibleSize();
	auto map = Sprite::create("backgrounds/japan.jfif");
	map->setPosition(screenSize.width / 2, screenSize.height / 2);
	map->setScale(SCENE_W / map->getContentSize().width, SCENE_H / map->getContentSize().height);
	addChild(map);

	// create Label
	auto label = Sprite::create("ui/woodboard.png");
	label->setPosition(Vec2(340, 430));
	addChild(label, 1);

	//auto spriteCharacter = Sprite::create("Idle__000.png");
	spriteCharacter = Sprite::create("player/play.png");
	spriteCharacter->setPosition(90, 200);
	spriteCharacter->setScale(0.8f);
	addChild(spriteCharacter,2);
	
	// create buton back
	auto backButton = ui::Button::create();
	backButton->loadTextureNormal("backnormal.png", ui::Widget::TextureResType::PLIST);
	backButton->loadTexturePressed("backpress.png", ui::Widget::TextureResType::PLIST);
	backButton->setPosition(Vec2(50, 430));
	//backButton->setScale(0.8f);
	addChild(backButton);
	backButton->addClickEventListener([&](Ref* render)
	{
		if (GameSetting::getInstance()->back == 0)
		{
			Director::getInstance()->replaceScene(TransitionFlipAngular::create(0.5f, MainMenuScene::createScene()));
		}
		else
		{
			Director::getInstance()->replaceScene(TransitionFlipAngular::create(0.5f, WaitScene::createScene()));
		}
	});
	
	//create money money.png
	auto spriteMoney = Sprite::createWithSpriteFrameName("iconmoney.png");
	spriteMoney->setPosition(690, 450);
	addChild(spriteMoney,3);

	//Lay ra so tien hien tai va ep kieu ve string
	char str[20];
	sprintf(str, "%d", gameSetting->getMoney());

	// create label money
	money = Label::create("", "fonts/Marker Felt.ttf", 24);
	money->setString(str);
	money->setColor(Color3B::RED);
	money->setPosition(730, 450);
	addChild(money, 2);

	//blood image
	auto bloodImage = Sprite::createWithSpriteFrameName("bloodnormal.png");
	bloodImage->setPosition(Vec2(600,450));
	bloodImage->setScale(0.8f);
	addChild(bloodImage,3);

	//Label for blood
	sprintf(str, "%d", gameSetting->getBloodSpell());

	numberSpell = Label::create("", "fonts/Marker Felt.ttf", 24);
	numberSpell->setString(str);
	numberSpell->setColor(Color3B::RED);
	numberSpell->setPosition(Vec2(640, 450));
	addChild(numberSpell, 3);

	//kunai image
	auto kunaiImage = Sprite::createWithSpriteFrameName("kunainormal.png");
	kunaiImage->setPosition(Vec2(500,450));
	kunaiImage->setScale(0.8f);
	addChild(kunaiImage, 2);

	//kunai Label
	sprintf(str, "%d", gameSetting->getNumberKunai());

	numberKunai = Label::create("", "fonts/Marker Felt.ttf", 24);
	numberKunai->setString(str);
	numberKunai->setColor(Color3B::RED);
	numberKunai->setPosition(Vec2(540, 450));
	addChild(numberKunai, 3);

	createItemButton();

	createBoardShop();

	return true;

}


void ShopItem::createItemButton()
{

	// create blood item
	auto bloodItem = ui::Button::create("ImageShop/blood.png");
	bloodItem->setPosition(Vec2(250, 280));
	bloodItem->setScale(0.6f);
	bloodItem->addClickEventListener([&](Ref* event)
	{
		dislayBoardShop();
		price = 50;
		type = 1;
	});
	addChild(bloodItem);

	// create sword item
	auto kunaiItem = ui::Button::create("ImageShop/kunai.png");

	kunaiItem->setPosition(Vec2(360, 280));
	kunaiItem->setScale(0.6f);
	kunaiItem->addClickEventListener([&](Ref* render)
	{
		dislayBoardShop();
		price = 50;
		type = 3;
	});

	addChild(kunaiItem);

	//create armor item
	auto armorItem = ui::Button::create("ImageShop/giap.png");
	armorItem->setPosition(Vec2(470, 280));
	armorItem->setScale(0.6f);
	armorItem->addClickEventListener([&](Ref* event)
	{
		dislayBoardShop();
		price = 100;
		type = 2;
	});
	addChild(armorItem);

	
}

void ShopItem::createBoardShop()
{

	//create board Item
	spritebg = Sprite::create("ImageShop/123.png");
	spritebg->setScale(0.9f);
	spritebg->setPosition(Vec2(610, 200));
	spritebg->setVisible(false);
	addChild(spritebg, 2);

	numberItem = Label::create("", "fonts/Marker Felt.ttf", 15);
	numberItem->setString("0");
	numberItem->setPosition(Vec2(spritebg->getContentSize().width/2, spritebg->getContentSize().height*0.6f));
	spritebg->addChild(numberItem, 2);

	totalMoney = Label::create("", "fonts/Marker Felt.ttf", 15);
	totalMoney->setString("0");
	totalMoney->setPosition(Vec2(spritebg->getContentSize().width*0.5f, spritebg->getContentSize().height*0.4f));
	spritebg->addChild(totalMoney, 2);

	// create  buton Buy
	auto btnBuy = ui::Button::create("ImageShop/btnBuy.png");
	btnBuy->setPosition(Vec2(spritebg->getContentSize().width*0.5, 40.0f));
	//btnBuy->setScale(0.2f);
	btnBuy->addClickEventListener([&](Ref* render)
	{
		if (gameSetting->getMoney() >= price*number)
		{
			switch (type)
			{
			case 1:
				if (gameSetting->getBloodSpell() + number <= 5)
				{
					gameSetting->setMoney(gameSetting->getMoney() - price*number);
					gameSetting->setBloodSpell(gameSetting->getBloodSpell() + number);
				}
				break;
			case 2:
				break;
			case 3:
				gameSetting->setMoney(gameSetting->getMoney() - price*number);
				gameSetting->setNumberKunai(gameSetting->getNumberKunai() + number);
				break;
			default:
				break;
			}
		}

		gameSetting->saveData();
		setLabel();
	});
	spritebg->addChild(btnBuy, 2);

	//create button close board
	auto btnexit = ui::Button::create("ImageShop/closenormal.png", "ImageShop/closepress.png");
	btnexit->setPosition(spritebg->getContentSize() - Size(20.0f, 20.0f));
	btnexit->addClickEventListener([&](Ref* render)
	{
		spritebg->setVisible(false);
	});
	spritebg->addChild(btnexit, 2);

	//create button increase
	auto btnAdd = ui::Button::create("ImageShop/add.png");
	btnAdd->setPosition(Vec2(spritebg->getContentSize().width - 40.0f, spritebg->getContentSize().height * 0.6f));
	btnAdd->addClickEventListener([&](Ref* render)
	{
		if (type == 1 && gameSetting->getBloodSpell() + number >= 5)
		{

		}
		else
		{
			number++;
		}
		
		setLabel();
	});
	spritebg->addChild(btnAdd, 2);

	//create button decrease
	auto btnMinus = ui::Button::create("ImageShop/minus.png");
	btnMinus->setPosition(Vec2(40.0f, spritebg->getContentSize().height * 0.6f));
	btnMinus->setScale(0.5f);
	btnMinus->addClickEventListener([&](Ref* render)
	{
		if (number > 0)
		{
			number--;
		}

		setLabel();
	});
	spritebg->addChild(btnMinus, 2);

	spritebg->setVisible(false);

	// create label luu y
	auto noteLabel = Label::create();
	noteLabel->setPosition(Vec2(spritebg->getContentSize().width / 2, spritebg->getContentSize().height * 0.3f));
	noteLabel->setString(" Note: total \n blood Spell <= 5");
	spritebg->addChild(noteLabel);
}

void ShopItem::dislayBoardShop()
{
	number = 0;

	spritebg->setVisible(true);

	setLabel();
}

void ShopItem::setLabel()
{
	char str[30];
	sprintf(str, "%d", number);

	numberItem->setString(str);

	sprintf(str, "Tong tien: %d", number*price);
	totalMoney->setString(str);

	sprintf(str, "%d", gameSetting->getMoney());
	money->setString(str);

	sprintf(str, "%d", gameSetting->getNumberKunai());
	numberKunai->setString(str);

	sprintf(str, "%d", gameSetting->getBloodSpell());
	numberSpell->setString(str);
}