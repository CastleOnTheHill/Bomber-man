#include "DesScene.h"
#include "prop.h"
#pragma execution_character_set("utf-8")

USING_NS_CC;

Scene* DesScene::createScene()
{
	return DesScene::create();
}

// on "init" you need to initialize your instance
bool DesScene::init()
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

	ValueMap map = FileUtils::getInstance()->getValueMapFromFile("des.xml");
	auto prop = map.at("prop").asString();
	auto back = map.at("back").asString();

	auto backButton = MenuItemFont::create(back, [](Ref* pSender) {
		Director::getInstance()->popScene();
	});
	if (backButton) {
		float x = 100;
		float y = visibleSize.height - 20;
		backButton->setPosition(Vec2(x, y));
		backButton->setColor(Color3B(0, 0, 0));
	}

	auto nextButton = MenuItemFont::create(prop, [](Ref* pSender) {
		Director::getInstance()->pushScene(prop::createScene());
	});
	if (nextButton) {
		float x = visibleSize.width - 100;
		float y = visibleSize.height -20;
		nextButton->setPosition(Vec2(x, y));
		nextButton->setColor(Color3B(0, 0, 0));
	}

	auto menu = Menu::create(backButton, nextButton, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	ValueMap p_map = FileUtils::getInstance()->getValueMapFromFile("description.xml");
	auto modelb = p_map.at("modelb").asString();
	auto move = p_map.at("move").asString();
	auto boom = p_map.at("boom").asString();
	auto player1 = p_map.at("playera").asString();
	auto player2 = p_map.at("playerb").asString();

	auto label_2p = Label::createWithTTF(modelb, "fonts/simsun.ttc", 40);
	label_2p->setColor(Color3B(0, 0, 0));
	label_2p->setPosition(visibleSize.width / 2, visibleSize.height - 80);
	this->addChild(label_2p);

	// 2pÄ£Ê½½éÉÜ
	//1p
	auto player_1 = Label::createWithTTF(player1, "fonts/simsun.ttc", 30);
	player_1->setColor(Color3B(0, 0, 0));
	player_1->setPosition(370, visibleSize.height - 220);
	this->addChild(player_1);

	auto label_move_1 = Label::createWithTTF(move, "fonts/simsun.ttc", 30);
	label_move_1->setColor(Color3B(0, 0, 0));
	label_move_1->setPosition(220, visibleSize.height - 300);
	this->addChild(label_move_1);

	auto button_left_1 = Sprite::create("A.png");
	button_left_1->setContentSize(Size(70, 70));
	button_left_1->setPosition(Vec2(300, 310));
	this->addChild(button_left_1);

	auto button_up_1 = Sprite::create("W.png");
	button_up_1->setContentSize(Size(70, 70));
	button_up_1->setPosition(Vec2(365, 370));
	this->addChild(button_up_1);

	auto button_right_1 = Sprite::create("D.png");
	button_right_1->setContentSize(Size(70, 70));
	button_right_1->setPosition(Vec2(430, 310));
	this->addChild(button_right_1);

	auto button_down_1 = Sprite::create("S.png");
	button_down_1->setContentSize(Size(70, 70));
	button_down_1->setPosition(Vec2(365, 310));
	this->addChild(button_down_1);

	auto label_boom_1 = Label::createWithTTF(boom, "fonts/simsun.ttc", 30);
	label_boom_1->setColor(Color3B(0, 0, 0));
	label_boom_1->setPosition(220, visibleSize.height - 420);
	this->addChild(label_boom_1);

	auto space_2 = Sprite::create("space.png");
	space_2->setContentSize(Size(190, 62));
	space_2->setPosition(Vec2(370, visibleSize.height - 420));
	this->addChild(space_2);

	// 2p
	auto player_2 = Label::createWithTTF(player2, "fonts/simsun.ttc", 30);
	player_2->setColor(Color3B(0, 0, 0));
	player_2->setPosition(700, visibleSize.height - 220);
	this->addChild(player_2);

	auto label_move_2 = Label::createWithTTF(move, "fonts/simsun.ttc", 30);
	label_move_2->setColor(Color3B(0, 0, 0));
	label_move_2->setPosition(550, visibleSize.height - 300);
	this->addChild(label_move_2);

	auto button_left_2 = Sprite::create("left.png");
	button_left_2->setContentSize(Size(70, 70));
	button_left_2->setPosition(Vec2(630, 310));
	this->addChild(button_left_2);

	auto button_up_2 = Sprite::create("up.png");
	button_up_2->setContentSize(Size(70, 70));
	button_up_2->setPosition(Vec2(695, 370));
	this->addChild(button_up_2);

	auto button_right_2 = Sprite::create("right.png");
	button_right_2->setContentSize(Size(70, 70));
	button_right_2->setPosition(Vec2(760, 310));
	this->addChild(button_right_2);

	auto button_down_2 = Sprite::create("down.png");
	button_down_2->setContentSize(Size(70, 70));
	button_down_2->setPosition(Vec2(695, 310));
	this->addChild(button_down_2);

	auto label_boom_2 = Label::createWithTTF(boom, "fonts/simsun.ttc", 30);
	label_boom_2->setColor(Color3B(0, 0, 0));
	label_boom_2->setPosition(550, visibleSize.height - 420);
	this->addChild(label_boom_2);

	auto enter = Sprite::create("enter.png");
	enter->setContentSize(Size(160, 62));
	enter->setPosition(Vec2(700, visibleSize.height - 420));
	this->addChild(enter);

	return true;
}