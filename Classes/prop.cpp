#include "prop.h"
#pragma execution_character_set("utf-8")

USING_NS_CC;

Scene* prop::createScene()
{
	return prop::create();
}

// on "init" you need to initialize your instance
bool prop::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Scene::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto bg = Sprite::create("bg-5.jpg");
	bg->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
	bg->setContentSize(Size(visibleSize.width, visibleSize.height));
	this->addChild(bg, 0);

	ValueMap p_map = FileUtils::getInstance()->getValueMapFromFile("des.xml");
	auto game = p_map.at("game").asString();
	auto power = p_map.at("power").asString();
	auto shoes = p_map.at("shoes").asString();
	auto number = p_map.at("number").asString();
	auto prop = p_map.at("prop").asString();

	auto backButton = MenuItemFont::create(game, [](Ref* pSender) {
		Director::getInstance()->popScene();
	});
	if (backButton) {
		float x = visibleSize.width - 100;
		float y = visibleSize.height - 20;
		backButton->setPosition(Vec2(x, y));
		backButton->setColor(Color3B(0, 0, 0));
	}

	auto menu = Menu::create(backButton, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	auto label = Label::createWithTTF(prop, "fonts/simsun.ttc", 40);
	label->setColor(Color3B(0, 0, 0));
	label->setPosition(visibleSize.width/2, 550);
	this->addChild(label);

	auto prop_1 = Sprite::create("prop-1.png");
	prop_1->setContentSize(Size(80, 80));
	prop_1->setPosition(Vec2(300, 420));
	this->addChild(prop_1);

	auto prop_2 = Sprite::create("prop-2.png");
	prop_2->setContentSize(Size(80, 80));
	prop_2->setPosition(Vec2(300, 300));
	this->addChild(prop_2);

	auto prop_3 = Sprite::create("prop-3.png");
	prop_3->setContentSize(Size(80, 80));
	prop_3->setPosition(Vec2(300, 180));
	this->addChild(prop_3);

	auto label_power = Label::createWithTTF(power, "fonts/simsun.ttc", 36);
	label_power->setColor(Color3B(0, 0, 0));
	label_power->setPosition(500, 420);
	this->addChild(label_power);

	auto label_shoes = Label::createWithTTF(shoes, "fonts/simsun.ttc", 36);
	label_shoes->setColor(Color3B(0, 0, 0));
	label_shoes->setPosition(500, 180);
	this->addChild(label_shoes);

	auto label_number = Label::createWithTTF(number, "fonts/simsun.ttc", 36);
	label_number->setColor(Color3B(0, 0, 0));
	label_number->setPosition(500, 300);
	this->addChild(label_number);

	return true;
}