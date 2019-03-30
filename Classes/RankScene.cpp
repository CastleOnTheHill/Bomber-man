#include "RankScene.h"
#define database UserDefault::getInstance()
USING_NS_CC;

Scene* RankScene::createScene()
{
	return RankScene::create();
}

// on "init" you need to initialize your instance
bool RankScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Scene::init())
	{
		return false;
	}

	if (!database->getBoolForKey("isExist")) {
		database->setBoolForKey("isExist", true);
	}

	std::string notExit = database->getStringForKey("notExist");
	std::string name1 = database->getStringForKey("Name1");
	std::string name2 = database->getStringForKey("Name2");
	std::string name3 = database->getStringForKey("Name3");
	std::string name4 = database->getStringForKey("Name4");
	std::string name5 = database->getStringForKey("Name5");

	

	int score1 = database->getIntegerForKey("Score1");
	int score2 = database->getIntegerForKey("Score2");
	int score3 = database->getIntegerForKey("Score3");
	int score4 = database->getIntegerForKey("Score4");
	int score5 = database->getIntegerForKey("Score5");

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto bg = Sprite::create("bg-2.jpg");
	bg->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
	bg->setContentSize(Size(visibleSize.width, visibleSize.height));
	this->addChild(bg, 0);

	auto backButton = MenuItemFont::create("Back", [](Ref* pSender) {
		Director::getInstance()->popScene();
	});
	if (backButton) {
		float x = origin.x + 100;
		float y = origin.y + visibleSize.height - backButton->getContentSize().height / 2;
		backButton->setPosition(Vec2(x, y));
	}

	auto menu = Menu::create(backButton, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	auto title = Sprite::create("button.png");
	title->setPosition(Size(visibleSize.width/2, visibleSize.height-50));
	title->setContentSize(Size(200, 70));
	this->addChild(title);

	auto gold1 = Sprite::create("gold1.png");
	gold1 ->setPosition(Size(200, 480));
	gold1->setContentSize(Size(50, 70));
	this->addChild(gold1);
	if (score1 != 0) {
		auto label1 = Label::createWithTTF(name1, "fonts/STXINWEI.TTF", 30);

		auto label2 = Label::createWithTTF(std::to_string(score1), "fonts/STXINWEI.TTF", 30);
		label1->setPosition(Vec2(300, 480));
		label2->setPosition(Vec2(400, 480));
		this->addChild(label1);
		this->addChild(label2);
	}
	else {
		auto label = Label::createWithTTF(notExit, "fonts/STXINWEI.TTF", 30);
		label->setPosition(Vec2(350, 480));
		this->addChild(label);
	}

	auto gold2 = Sprite::create("gold2.png");
	gold2->setPosition(Size(200, 390));
	gold2->setContentSize(Size(50, 70));
	this->addChild(gold2);
	if (score2 != 0) {
		auto label = Label::createWithTTF(name2, "fonts/STXINWEI.TTF", 30);

		auto label2 = Label::createWithTTF(std::to_string(score2), "fonts/STXINWEI.TTF", 30);
		label->setPosition(Vec2(300, 390));
		label2->setPosition(Vec2(400, 390));
		this->addChild(label);
		this->addChild(label2);
	}
	else {
		auto label = Label::createWithTTF(notExit, "fonts/STXINWEI.TTF", 30);
		label->setPosition(Vec2(350, 390));
		this->addChild(label);
	}


	auto gold3 = Sprite::create("gold3.png");
	gold3->setPosition(Size(200, 300));
	gold3->setContentSize(Size(50, 70));
	this->addChild(gold3);

	if (score3 != 0) {
		auto label = Label::createWithTTF(name3, "fonts/STXINWEI.TTF", 30);

		auto label2 = Label::createWithTTF(std::to_string(score3), "fonts/STXINWEI.TTF", 30);
		label->setPosition(Vec2(300, 300));
		label2->setPosition(Vec2(400, 300));
		this->addChild(label);
		this->addChild(label2);
	}
	else {
		auto label = Label::createWithTTF(notExit, "fonts/STXINWEI.TTF", 30);
		label->setPosition(Vec2(350, 300));
		this->addChild(label);
	}

	auto good1 = Sprite::create("good.png");
	good1->setPosition(Size(200, 210));
	good1->setContentSize(Size(90, 80));
	this->addChild(good1);

	if (score4 != 0) {
		auto label = Label::createWithTTF(name4, "fonts/STXINWEI.TTF", 30);

		auto label2 = Label::createWithTTF(std::to_string(score4), "fonts/STXINWEI.TTF", 30);
		label->setPosition(Vec2(300, 210));
		label2->setPosition(Vec2(400, 210));
		this->addChild(label);
		this->addChild(label2);
	}
	else {
		auto label = Label::createWithTTF(notExit, "fonts/STXINWEI.TTF", 30);
		label->setPosition(Vec2(350, 210));
		this->addChild(label);
	}

	auto good2 = Sprite::create("good.png");
	good2->setPosition(Size(200, 120));
	good2->setContentSize(Size(90, 80));
	this->addChild(good2);
	
	if (score5 != 0) {
		auto label = Label::createWithTTF(name5, "fonts/STXINWEI.TTF", 30);

		auto label2 = Label::createWithTTF(std::to_string(score5), "fonts/STXINWEI.TTF", 30);
		label->setPosition(Vec2(300, 120));
		label2->setPosition(Vec2(400, 120));
		this->addChild(label);
		this->addChild(label2);
	}
	else {
		auto label = Label::createWithTTF(notExit, "fonts/STXINWEI.TTF", 30);
		label->setPosition(Vec2(350, 120));
		this->addChild(label);
	}

	return true;
}