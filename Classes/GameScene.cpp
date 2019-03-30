#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "extensions\cocos-ext.h"
USING_NS_CC_EXT;
using namespace CocosDenshion;
#define database UserDefault::getInstance()
USING_NS_CC;

Scene* GameScene::createScene()
{
	auto scene = Scene::createWithPhysics();

	scene->getPhysicsWorld()->setAutoStep(true);
	//debug
	auto layer = GameScene::create();
	layer->setPhyWorld(scene->getPhysicsWorld());
	//scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	scene->addChild(layer);
	return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	//add touch listener

	string s = database->getXMLFilePath();

	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();

	preloadMusic();
	SimpleAudioEngine::getInstance()->playBackgroundMusic("playingBgm.mp3");

	auto bg = Sprite::create("bg-1.jpg");
	bg->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
	bg->setContentSize(Size(visibleSize.width, visibleSize.height));
	this->addChild(bg, 0);

	auto lab = Label::createWithTTF("0", "fonts/arial.ttf", 36);
	lab->setPosition(visibleSize.width / 2, + visibleSize.height - 19);
	lab->setName("timer");
	this->addChild(lab, 1);
	
	if(!database->getBoolForKey("isExist")) {
		database->setBoolForKey("isExist", true);
	}


	blockSize = 45;
	P1MoveDriection = direction::down;
	P2MoveDriection = direction::up;
	P1IsMoving = false;
	P2IsMoving = false;
	initMap();
	addaddListener();
	loadAnimation();
	initAllEle();
	schedule(schedule_selector(GameScene::updateTime), 1.0f, kRepeatForever, 0);
	schedule(schedule_selector(GameScene::updateCustom), 0.4f, kRepeatForever, 0);
	return true;
}


void GameScene::initMap() {
	map = TMXTiledMap::create("newMap.tmx");
	map->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	map->setAnchorPoint(Vec2(0.5, 0.5));
	origin.x = visibleSize.width / 2 - blockSize * 6.5;
	origin.y = visibleSize.height / 2 - blockSize * 6;
	this->addChild(map, 0);
}

void GameScene::initAllEle()
{
	mapWidthBlock = 13;
	mapHeightBlock = 11;
	
	auto edgeSp = Sprite::create();
	auto bound = PhysicsBody::createEdgeBox(Size(mapWidthBlock * blockSize, mapHeightBlock * blockSize), PhysicsMaterial(0.0f, 0.0f, 0.0f));
	//bound->setContactTestBitmask(0xFFFFFFFF);
	bound->setDynamic(false);
	edgeSp->setPosition(origin.x + mapWidthBlock * blockSize / 2, origin.y + mapHeightBlock * blockSize / 2);
	edgeSp->setPhysicsBody(bound);
	addChild(edgeSp);

	blockCoor wallCoor[43] = {
		blockCoor(1, 1),blockCoor(1, 2),blockCoor(1, 3),blockCoor(1, 4),blockCoor(1, 5),blockCoor(1, 7),blockCoor(1, 8),blockCoor(1, 9),
		blockCoor(2, 1),blockCoor(2, 9),
		blockCoor(3, 1),blockCoor(3, 3),blockCoor(3, 5),blockCoor(3, 6),blockCoor(3, 7),blockCoor(3, 9),
		blockCoor(4, 7),
		blockCoor(5, 1),blockCoor(5, 3),blockCoor(5, 5),blockCoor(5, 7),blockCoor(5, 9),
		blockCoor(7, 1),blockCoor(7, 3),blockCoor(7, 5),blockCoor(7, 7),blockCoor(7, 9),
		blockCoor(8, 7),
		blockCoor(9, 1),blockCoor(9, 3),blockCoor(9, 5),blockCoor(9, 6),blockCoor(9, 7),blockCoor(9, 9),
		blockCoor(10, 3),blockCoor(10, 9),
		blockCoor(11, 1),blockCoor(11, 3),blockCoor(11, 5),blockCoor(11, 7),blockCoor(11, 8),blockCoor(11, 9),
		blockCoor(12, 3)
	};
	blockCoor boxesCoor[32] = {
		blockCoor(0, 4),blockCoor(0, 5),blockCoor(0, 7),blockCoor(0, 8),
		blockCoor(2, 3),blockCoor(2, 4),blockCoor(2, 5),
		blockCoor(3, 4),blockCoor(3, 10),
		blockCoor(4, 1),blockCoor(4, 5),blockCoor(4, 9),
		blockCoor(5, 2),blockCoor(5, 4),blockCoor(5, 6),blockCoor(5, 10),
		blockCoor(6, 9),blockCoor(6, 10),
		blockCoor(7, 2),blockCoor(7, 4),blockCoor(7, 6),
		blockCoor(8, 1),blockCoor(8, 3),blockCoor(8, 10),
		blockCoor(9, 2),
		blockCoor(10, 1), blockCoor(10, 6),
		blockCoor(11, 2), blockCoor(11, 6),
		blockCoor(12, 5),blockCoor(12, 7),blockCoor(12, 8)
	};

	for (int i = 0; i < 43; i++) {
		auto boundBody = PhysicsBody::createBox(Size(blockSize, blockSize), PhysicsMaterial(0.0f, 0.0f, 0.0f));
		Sprite* sp = Sprite::create();
		boundBody->setDynamic(false);
		boundBody->setContactTestBitmask(0xFFFFFFFF);
		boundBody->setGroup(-1);
		sp->setName("wall");
		sp->setPosition(converCoorToPos(wallCoor[i]));
		sp->setPhysicsBody(boundBody);
		walls.pushBack(sp);
		this->addChild(sp, 1);
	}

	for (int i = 0; i < 32; i++) {
		auto boundBody = PhysicsBody::createBox(Size(blockSize, blockSize), PhysicsMaterial(0.0f, 0.0f, 0.0f));
		Sprite* sp = Sprite::create("newResources.png", Rect(blockSize, blockSize, blockSize, blockSize));
		boundBody->setDynamic(false);
		boundBody->setContactTestBitmask(0xFFFFFFFF);
		boundBody->setGroup(-1);
		sp->setName("box");
		sp->setPosition(converCoorToPos(boxesCoor[i]));
		sp->setPhysicsBody(boundBody);
		boxes.pushBack(sp);
		this->addChild(sp, 1);
	}
	P1 = Sprite::create("resources.png", Rect(24, 138, 24, 33));
	auto boundBody1 = PhysicsBody::createBox(Size(24, 33), PhysicsMaterial(0.0f, 0.0f, 0.0f));
	boundBody1->setGravityEnable(false);
	boundBody1->setRotationEnable(false);
	boundBody1->setContactTestBitmask(0x1);
	boundBody1->setGroup(-2);
	P1->setPosition(converCoorToPos(blockCoor(0, 10)));
	P1->setPhysicsBody(boundBody1);
	addChild(P1, 1);

	P2 = Sprite::create("P2.png", Rect(0, 78, 29, 38));
	auto boundBody2 = PhysicsBody::createBox(Size(29, 38), PhysicsMaterial(0.0f, 0.0f, 0.0f));
	boundBody2->setGravityEnable(false);
	boundBody2->setRotationEnable(false);
	boundBody2->setGroup(-2);
	boundBody2->setContactTestBitmask(0x1);
	P2->setPosition(converCoorToPos(blockCoor(0, 3)));
	P2->setPhysicsBody(boundBody2);
	addChild(P2, 1);
}


void GameScene::updateCustom(float dt)
{
	if (P1IsMoving) {
		string animationName;
		switch (P1MoveDriection)
		{
		case up:
			animationName = "P1movingUp";
			break;
		case down:
			animationName = "P1movingDown";
			break;
		case left:
			animationName = "P1movingLeft";
			break;
		case right:
			animationName = "P1movingRight";
			break;
		default:
			break;
		}
		P1->runAction(Animate::create(AnimationCache::getInstance()->getAnimation(animationName)));
	}
	if (P2IsMoving) {
		string animationName;
		switch (P2MoveDriection)
		{
		case up:
			animationName = "P2movingUp";
			break;
		case down:
			animationName = "P2movingDown";
			break;
		case left:
			animationName = "P2movingLeft";
			break;
		case right:
			animationName = "P2movingRight";
			break;
		default:
			break;
		}
		P2->runAction(Animate::create(AnimationCache::getInstance()->getAnimation(animationName)));
	}
}

void GameScene::onKeyPressed(EventKeyboard::KeyCode code, Event * event)
{

	switch (code) {
	case cocos2d::EventKeyboard::KeyCode::KEY_A:
		P1->getPhysicsBody()->setVelocity(Vec2(P1State.speed * (-1), 0));
		P1IsMoving = true;
		P1MoveDriection = direction::left;
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_D:
		P1->getPhysicsBody()->setVelocity(Vec2(P1State.speed, 0));
		P1IsMoving = true;
		P1MoveDriection = direction::right;
		break;

	case cocos2d::EventKeyboard::KeyCode::KEY_S:
		P1->getPhysicsBody()->setVelocity(Vec2(0, P1State.speed * (-1)));
		P1IsMoving = true;
		P1MoveDriection = direction::down;
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_W:
		P1->getPhysicsBody()->setVelocity(Vec2(0, P1State.speed));
		P1IsMoving = true;
		P1MoveDriection = direction::up;
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_SPACE:
		if (PlaceBomb(P1State, P1->getPosition())) {
			SimpleAudioEngine::getInstance()->playEffect("place.mp3");
		}
		break;


	case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		P2->getPhysicsBody()->setVelocity(Vec2(P2State.speed * (-1), 0));
		P2MoveDriection = direction::left;
		P2IsMoving = true;
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		P2->getPhysicsBody()->setVelocity(Vec2(P2State.speed, 0));
		P2MoveDriection = direction::right;
		P2IsMoving = true;
		break;

	case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		P2->getPhysicsBody()->setVelocity(Vec2(0, P2State.speed * (-1)));
		P2MoveDriection = direction::down;
		P2IsMoving = true;
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW:
		P2->getPhysicsBody()->setVelocity(Vec2(0, P2State.speed));
		P2MoveDriection = direction::up;
		P2IsMoving = true;
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_ENTER:
		if (PlaceBomb(P2State, P2->getPosition())) {
			SimpleAudioEngine::getInstance()->playEffect("place.mp3");
		}
		break;
	default:
		break;
	}

}

void GameScene::onKeyReleased(EventKeyboard::KeyCode code, Event * event)
{
	switch (code) {
	case cocos2d::EventKeyboard::KeyCode::KEY_W:
	case cocos2d::EventKeyboard::KeyCode::KEY_A:
	case cocos2d::EventKeyboard::KeyCode::KEY_S:
	case cocos2d::EventKeyboard::KeyCode::KEY_D:
		P1IsMoving = false;
		P1->getPhysicsBody()->setVelocity(Vec2(0, 0));
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
	case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
	case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:
	case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW:
		P2IsMoving = false;
		P2->getPhysicsBody()->setVelocity(Vec2(0, 0));
		break;
	default:
		break;
	}
}

bool GameScene::onConcactBegin(PhysicsContact & contact)
{
	auto body1 = contact.getShapeA()->getBody();
	auto body2 = contact.getShapeB()->getBody();
	auto sp1 = (Sprite*)body1->getNode();
	auto sp2 = (Sprite*)body2->getNode();
	Sprite* object, *player;
	if (sp1 == P1 || sp1 == P2) {
		object = sp2;
		player = sp1;
	}
	else {
		object = sp1;
		player = sp2;
	}

	if (object->getName() == "power") {
		getPlayState(player).power += 2;
		object->removeFromParent();
	}
	else if (object->getName() == "num") {
		getPlayState(player).bombNum += 1;
		object->removeFromParent();
	}
	else if (object->getName() == "speed") {
		getPlayState(player).speed += 50;
		object->removeFromParent();
	}
	return true;
}

void GameScene::preloadMusic()
{
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic("playingBgm.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("place.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("gameOver.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("explode.wav");
}

void GameScene::GameOver() {
	_eventDispatcher->removeAllEventListeners();
	SimpleAudioEngine::getInstance()->pauseBackgroundMusic();

	ValueMap p_map = FileUtils::getInstance()->getValueMapFromFile("game.xml");
	auto name = p_map.at("name").asString();
	auto gameover1 = p_map.at("gameover1").asString();
	auto gameover2 = p_map.at("gameover2").asString();
	auto replayword = p_map.at("replay").asString();
	auto exitword = p_map.at("exit").asString();
	auto yes = p_map.at("yes").asString();

	auto label = Label::createWithTTF(name, "fonts/STXINWEI.TTF", 30);
	label->setColor(Color3B(255, 255, 255));
	label->setPosition(visibleSize.width / 2, visibleSize.height *0.7);
	this->addChild(label, 3);

	auto editbox1 = EditBox::create(Size(300, 70), Scale9Sprite::create("editbox.png"));
	editbox1->setAnchorPoint(Point(0, 0));
	editbox1->setName("editbox");
	editbox1->setPosition(Point(visibleSize.width*0.34, visibleSize.height*0.55));
	//editbox1->setPlaceHolder("Name:");//占位字符
	editbox1->setMaxLength(8);
	editbox1->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
	editbox1->setFontColor(Color3B::WHITE);//设置输入字体的颜色
	editbox1->setTag(1);
	this->addChild(editbox1, 2);

	auto label4 = Label::createWithTTF(yes, "fonts/STXINWEI.TTF", 30);
	label4->setColor(Color3B(255, 255, 255));
	auto yesbtn = MenuItemLabel::create(label4, CC_CALLBACK_1(GameScene::yesBtnCallback, this));
	Menu* yesmenu = Menu::create(yesbtn, NULL);
	yesmenu->setPosition(visibleSize.width / 2, visibleSize.height *0.5);
	yesmenu->setName("yesmenu");
	this->addChild(yesmenu, 3);

	if (winner == "P1") {
		auto label1 = Label::createWithTTF(gameover1, "fonts/STXINWEI.TTF", 60);
		label1->setColor(Color3B(255, 255, 255));
		label1->setPosition(visibleSize.width / 2, visibleSize.height *0.8);
		this->addChild(label1, 3);
	}
	else {
		auto label1 = Label::createWithTTF(gameover2, "fonts/STXINWEI.TTF", 60);
		label1->setColor(Color3B(255, 255, 255));
		label1->setPosition(visibleSize.width / 2, visibleSize.height *0.8);
		this->addChild(label1, 3);
	}

	auto label2 = Label::createWithTTF(replayword, "fonts/STXINWEI.TTF", 40);
	label2->setColor(Color3B(255, 255, 255));
	auto replayBtn = MenuItemLabel::create(label2, CC_CALLBACK_1(GameScene::replayCallback, this));
	Menu* replay = Menu::create(replayBtn, NULL);
	replay->setPosition(visibleSize.width / 2 - 80, visibleSize.height / 2 - 100);
	this->addChild(replay, 3);

	auto label3 = Label::createWithTTF(exitword, "fonts/STXINWEI.TTF", 40);
	label3->setColor(Color3B(255, 255, 255));
	auto exitBtn = MenuItemLabel::create(label3, CC_CALLBACK_1(GameScene::exitCallback, this));
	Menu* exit = Menu::create(exitBtn, NULL);
	exit->setPosition(visibleSize.width / 2 + 90, visibleSize.height / 2 - 100);
	this->addChild(exit, 3);
}

void GameScene::yesBtnCallback(Ref * pSender) {
	EditBox* ed = (EditBox*)this->getChildByName("editbox");
	Menu* me =  (Menu*)this->getChildByName("yesmenu");
	string winnerName = ed->getText();
	auto timer = (Label*)this->getChildByName("timer");
	std::stringstream stream(timer->getString());
	std::stringstream ss;
	int tt;
	stream >> tt;
	
	int Score1 = database->getIntegerForKey("Score1");
	int Score2 = database->getIntegerForKey("Score2");
	int Score3 = database->getIntegerForKey("Score3");
	int Score4 = database->getIntegerForKey("Score4");
	int Score5 = database->getIntegerForKey("Score5");
	int score[5] = { Score1 , Score2, Score3, Score4, Score5 };
	char temp[10], temp2[10];
	for (int i = 0; i < 5; i++)
	{
		if (score[i] == 0 || score[i] > tt) {
			sprintf(temp, "Name%d", i + 1);
			sprintf(temp2, "Score%d", i + 1);
			database->setStringForKey(temp, winnerName);
			database->setIntegerForKey(temp2, tt);
			break;
		}
	}
	me->setEnabled(false);
}


void GameScene::exitCallback(Ref * pSender) {
	Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void GameScene::replayCallback(Ref * pSender) {
	Director::getInstance()->replaceScene(GameScene::createScene());
}


bool GameScene::PlaceBomb(playerState &state, Vec2 pos)
{
	if (state.bombNum > 0) {
		state.bombNum--;
		auto bomb = Sprite::create("bom.png", Rect(0, 0, 24, 24));
		bomb->setUserData(&(state));
		bomb->setPosition(converCoorToPos(standarPos(pos)));
		this->addChild(bomb, 1);
		bomb->runAction(
			Sequence::create(
				Animate::create(AnimationCache::getInstance()->getAnimation("bombGoningToExp")),
				CallFunc::create([=] {
					EventCustom e("explode");
					e.setUserData(bomb);
					Director::getInstance()->getEventDispatcher()->dispatchEvent(&e);
					SimpleAudioEngine::getInstance()->playEffect("explode.wav");
				}),
				nullptr
			)
		);
		return true;
	}
	else {
		return false;
	}
}


void GameScene::updateTime(float dt)
{
	auto timer = (Label*)this->getChildByName("timer");
	std::stringstream stream(timer->getString());
	std::stringstream ss;
	int tt;
	stream >> tt;
	tt++;
	ss << tt;
	timer->setString(ss.str());
}

void GameScene::loadAnimation()
{
	Animation* bombGoningToExp = Animation::create();

	bombGoningToExp->addSpriteFrame(SpriteFrame::create("bom.png", Rect(0 + 2 * 24, 0, 20, 24)));
	bombGoningToExp->addSpriteFrame(SpriteFrame::create("bom.png", Rect(0 + 1 * 24, 0, 24, 24)));
	bombGoningToExp->addSpriteFrame(SpriteFrame::create("bom.png", Rect(0 + 0 * 24, 0, 24, 22)));

	bombGoningToExp->setDelayPerUnit(0.5);
	AnimationCache::getInstance()->addAnimation(bombGoningToExp, "bombGoningToExp");

	Animation* bombExplodingCenter = Animation::create();

	bombExplodingCenter->addSpriteFrame(SpriteFrame::create("newBom.png", Rect(0, 45, 45, 45)));
	bombExplodingCenter->addSpriteFrame(SpriteFrame::create("newBom.png", Rect(0, 90, 45, 45)));
	bombExplodingCenter->addSpriteFrame(SpriteFrame::create("newBom.png", Rect(0, 135, 45, 45)));
	bombExplodingCenter->addSpriteFrame(SpriteFrame::create("newBom.png", Rect(0, 180, 45, 45)));
	bombExplodingCenter->setDelayPerUnit(0.25);

	AnimationCache::getInstance()->addAnimation(bombExplodingCenter, "bombExplodingCenter");


	Animation* bombExplodingHorizontal = Animation::create();

	bombExplodingHorizontal->addSpriteFrame(SpriteFrame::create("newBom.png", Rect(230, 50, 40, 45)));
	bombExplodingHorizontal->addSpriteFrame(SpriteFrame::create("newBom.png", Rect(230, 95, 40, 45)));
	bombExplodingHorizontal->addSpriteFrame(SpriteFrame::create("newBom.png", Rect(230, 140, 40, 45)));
	bombExplodingHorizontal->addSpriteFrame(SpriteFrame::create("newBom.png", Rect(230, 180, 40, 45)));
	bombExplodingHorizontal->setDelayPerUnit(0.25);

	AnimationCache::getInstance()->addAnimation(bombExplodingHorizontal, "bombExplodingHorizontal");

	Animation* bombExplodingVertical = Animation::create();

	bombExplodingVertical->addSpriteFrame(SpriteFrame::create("newBom.png", Rect(120, 50, 45, 31)));
	bombExplodingVertical->addSpriteFrame(SpriteFrame::create("newBom.png", Rect(120, 95, 45, 31)));
	bombExplodingVertical->addSpriteFrame(SpriteFrame::create("newBom.png", Rect(120, 140, 45, 31)));
	bombExplodingVertical->addSpriteFrame(SpriteFrame::create("newBom.png", Rect(120, 185, 45, 31)));
	bombExplodingVertical->setDelayPerUnit(0.25);

	AnimationCache::getInstance()->addAnimation(bombExplodingVertical, "bombExplodingVertical");

	Animation * P1movingDown = Animation::create();

	P1movingDown->addSpriteFrame(SpriteFrame::create("resources.png", Rect(0, 138, 24, 33)));
	P1movingDown->addSpriteFrame(SpriteFrame::create("resources.png", Rect(24, 138, 24, 33)));
	P1movingDown->addSpriteFrame(SpriteFrame::create("resources.png", Rect(48, 138, 24, 33)));
	P1movingDown->addSpriteFrame(SpriteFrame::create("resources.png", Rect(0, 138, 24, 33)));
	P1movingDown->setDelayPerUnit(0.1);
	AnimationCache::getInstance()->addAnimation(P1movingDown, "P1movingDown");

	Animation * P1movingUp = Animation::create();

	P1movingUp->addSpriteFrame(SpriteFrame::create("resources.png", Rect(72 - 3, 138, 24, 33)));
	P1movingUp->addSpriteFrame(SpriteFrame::create("resources.png", Rect(96 - 3, 138, 24, 33)));
	P1movingUp->addSpriteFrame(SpriteFrame::create("resources.png", Rect(120 - 3, 138, 24, 33)));
	P1movingUp->addSpriteFrame(SpriteFrame::create("resources.png", Rect(72 - 3, 138, 24, 33)));
	P1movingUp->setDelayPerUnit(0.1);
	AnimationCache::getInstance()->addAnimation(P1movingUp, "P1movingUp");

	Animation * P1movingLeft = Animation::create();

	P1movingLeft->addSpriteFrame(SpriteFrame::create("resources.png", Rect(144 - 8, 138, 24, 33)));
	P1movingLeft->addSpriteFrame(SpriteFrame::create("resources.png", Rect(168 - 8, 138, 24, 33)));
	P1movingLeft->addSpriteFrame(SpriteFrame::create("resources.png", Rect(192 - 8, 138, 24, 33)));
	P1movingLeft->addSpriteFrame(SpriteFrame::create("resources.png", Rect(144 - 8, 138, 24, 33)));
	P1movingLeft->setDelayPerUnit(0.1);
	AnimationCache::getInstance()->addAnimation(P1movingLeft, "P1movingLeft");


	Animation * P1movingRight = Animation::create();

	P1movingRight->addSpriteFrame(SpriteFrame::create("resources.png", Rect(0, 173, 24, 33)));
	P1movingRight->addSpriteFrame(SpriteFrame::create("resources.png", Rect(24, 173, 24, 33)));
	P1movingRight->addSpriteFrame(SpriteFrame::create("resources.png", Rect(48, 173, 24, 33)));
	P1movingRight->addSpriteFrame(SpriteFrame::create("resources.png", Rect(0, 173, 24, 33)));
	P1movingRight->setDelayPerUnit(0.1);
	AnimationCache::getInstance()->addAnimation(P1movingRight, "P1movingRight");

	Animation* P2movingDown = Animation::create();
	P2movingDown->addSpriteFrame(SpriteFrame::create("P2.png", Rect(0, 0, 29, 38)));
	P2movingDown->addSpriteFrame(SpriteFrame::create("P2.png", Rect(30, 0, 29, 38)));
	P2movingDown->addSpriteFrame(SpriteFrame::create("P2.png", Rect(60, 0, 29, 38)));
	P2movingDown->addSpriteFrame(SpriteFrame::create("P2.png", Rect(0, 0, 29, 38)));
	P2movingDown->setDelayPerUnit(0.1);
	AnimationCache::getInstance()->addAnimation(P2movingDown, "P2movingDown");

	Animation* P2movingUp = Animation::create();
	P2movingUp->addSpriteFrame(SpriteFrame::create("P2.png", Rect(0, 78, 29, 38)));
	P2movingUp->addSpriteFrame(SpriteFrame::create("P2.png", Rect(33, 78, 29, 38)));
	P2movingUp->addSpriteFrame(SpriteFrame::create("P2.png", Rect(63, 78, 29, 38)));
	P2movingUp->addSpriteFrame(SpriteFrame::create("P2.png", Rect(0, 78, 29, 38)));
	P2movingUp->setDelayPerUnit(0.1);
	AnimationCache::getInstance()->addAnimation(P2movingUp, "P2movingUp");

	Animation* P2movingLeft = Animation::create();
	P2movingLeft->addSpriteFrame(SpriteFrame::create("P2.png", Rect(0, 40, 29, 38)));
	P2movingLeft->addSpriteFrame(SpriteFrame::create("P2.png", Rect(33, 40, 29, 38)));
	P2movingLeft->addSpriteFrame(SpriteFrame::create("P2.png", Rect(63, 40, 29, 38)));
	P2movingLeft->addSpriteFrame(SpriteFrame::create("P2.png", Rect(0, 40, 29, 38)));
	P2movingLeft->setDelayPerUnit(0.1);
	AnimationCache::getInstance()->addAnimation(P2movingLeft, "P2movingLeft");

	Animation* P2movingRight = Animation::create();
	P2movingRight->addSpriteFrame(SpriteFrame::create("P2.png", Rect(0, 116, 29, 38)));
	P2movingRight->addSpriteFrame(SpriteFrame::create("P2.png", Rect(32, 116, 29, 38)));
	P2movingRight->addSpriteFrame(SpriteFrame::create("P2.png", Rect(62, 116, 29, 38)));
	P2movingRight->addSpriteFrame(SpriteFrame::create("P2.png", Rect(0, 116, 29, 38)));
	P2movingRight->setDelayPerUnit(0.1);
	AnimationCache::getInstance()->addAnimation(P2movingRight, "P2movingRight");
}

blockCoor GameScene::standarPos(Vec2 before) // 将任意位置转化为格子位置
{
	int count = 0;
	while (origin.x + blockSize * count < before.x)
	{
		count++;
	}
	
	int count2 = 0;
	while (origin.y + blockSize * count2 < before.y)
	{
		count2++;
	}

	return blockCoor(count-1, count2 - 1);
}

void GameScene::addaddListener()
{

	auto keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyPressed = CC_CALLBACK_2(GameScene::onKeyPressed, this);
	keyboardListener->onKeyReleased = CC_CALLBACK_2(GameScene::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);

	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(GameScene::onConcactBegin, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

	_eventDispatcher->addCustomEventListener("explode", CC_CALLBACK_1(GameScene::explodeHandle, this));
	_eventDispatcher->addCustomEventListener("boxDestroy", CC_CALLBACK_1(GameScene::boxDestroyHandle, this));
	_eventDispatcher->addCustomEventListener("gameOver", CC_CALLBACK_1(GameScene::gameOverHandle, this));
}

bool GameScene::isInMap(Vec2 point)
{
	if (point.x < origin.x || point.x > origin.x + mapWidthBlock * blockSize
		|| point.y < origin.y || point.y > origin.y + mapHeightBlock * blockSize) {
		return false;
	}
	return true;
}

void GameScene::explodeHandle(EventCustom * event)
{

	EventCustom signal("gameOver");
	auto bomb = (Sprite*)event->getUserData();
	Vector<Sprite*> destroy;
	playerState* state = (playerState *)bomb->getUserData();
	auto pos = bomb->getPosition();

	if (converCoorToPos(standarPos(P1->getPosition())) == pos) {
		
		signal.setUserData("P2");
		_eventDispatcher->dispatchEvent(&signal);
		bomb->removeFromParent();
		return;
	}
	else if (converCoorToPos(standarPos(P2->getPosition())) == pos) {
		signal.setUserData("P1");
		_eventDispatcher->dispatchEvent(&signal);
		bomb->removeFromParent();
		return;
	}
	int upLength = state->power / 2, downLength = state->power / 2, leftLength = state->power / 2, rightLength = state->power / 2;
	for (int i = 0; i < state->power / 2; i++)
	{

		bool flag = false;
		pos = GetNextPos(pos, direction::up);
		if (converCoorToPos(standarPos(P1->getPosition())) == pos) {

			signal.setUserData("P2");
			_eventDispatcher->dispatchEvent(&signal);
			bomb->removeFromParent();
			return;
		}
		else if (converCoorToPos(standarPos(P2->getPosition())) == pos) {
			signal.setUserData("P1");
			_eventDispatcher->dispatchEvent(&signal);
			bomb->removeFromParent();
			return;
		}
		if (!isInMap(pos)) {
			upLength = i;
			break;
		}
		for (auto box : boxes) {
			if (box->getBoundingBox().containsPoint(pos)) {
				upLength = i;
				destroy.pushBack(box);
				flag = true;
				break;
			}
		}
		for (auto wall : walls) {
			if (wall->getBoundingBox().containsPoint(pos)) {
				upLength = i;
				flag = true;
				break;
			}
		}
		if (flag) {
			break;
		}
	}

	pos = bomb->getPosition();
	for (int i = 0; i < state->power / 2; i++)
	{
		bool flag = false;
		pos = GetNextPos(pos, direction::down);
		if (converCoorToPos(standarPos(P1->getPosition())) == pos) {

			signal.setUserData("P2");
			_eventDispatcher->dispatchEvent(&signal);
			bomb->removeFromParent();
			return;
		}
		else if (converCoorToPos(standarPos(P2->getPosition())) == pos) {
			signal.setUserData("P1");
			_eventDispatcher->dispatchEvent(&signal);
			bomb->removeFromParent();
			return;
		}
		if (!isInMap(pos)) {
			downLength = i;
			break;
		}
		for (auto box : boxes) {
			if (box->getBoundingBox().containsPoint(pos)) {
				downLength = i;
				destroy.pushBack(box);
				flag = true;
				break;
			}
		}
		for (auto wall : walls) {
			if (wall->getBoundingBox().containsPoint(pos)) {
				downLength = i;
				flag = true;
				break;
			}
		}
		if (flag) {
			break;
		}
	}

	pos = bomb->getPosition();
	for (int i = 0; i < state->power / 2; i++)
	{
		bool flag = false;
		pos = GetNextPos(pos, direction::left);
		if (converCoorToPos(standarPos(P1->getPosition())) == pos) {

			signal.setUserData("P2");
			_eventDispatcher->dispatchEvent(&signal);
			bomb->removeFromParent();
			return;
		}
		else if (converCoorToPos(standarPos(P2->getPosition())) == pos) {
			signal.setUserData("P1");
			_eventDispatcher->dispatchEvent(&signal);
			bomb->removeFromParent();
			return;
		}
		if (!isInMap(pos)) {
			leftLength = i;
			break;
		}
		for (auto box : boxes) {
			if (box->getBoundingBox().containsPoint(pos)) {
				leftLength = i;
				destroy.pushBack(box);
				flag = true;
				break;
			}
		}
		for (auto wall : walls) {
			if (wall->getBoundingBox().containsPoint(pos)) {
				leftLength = i;
				flag = true;
				break;
			}
		}
		if (flag) {
			break;
		}
	}

	pos = bomb->getPosition();
	for (int i = 0; i < state->power / 2; i++)
	{
		bool flag = false;
		pos = GetNextPos(pos, direction::right);
		if (converCoorToPos(standarPos(P1->getPosition())) == pos) {

			signal.setUserData("P2");
			_eventDispatcher->dispatchEvent(&signal);
			bomb->removeFromParent();
			return;
		}
		else if (converCoorToPos(standarPos(P2->getPosition())) == pos) {
			signal.setUserData("P1");
			_eventDispatcher->dispatchEvent(&signal);
			bomb->removeFromParent();
			return;
		}
		if (!isInMap(pos)) {
			rightLength = i;
			break;
		}
		for (auto box : boxes) {
			if (box->getBoundingBox().containsPoint(pos)) {
				rightLength = i;
				destroy.pushBack(box);
				flag = true;
				break;
			}
		}
		for (auto wall : walls) {
			if (wall->getBoundingBox().containsPoint(pos)) {
				rightLength = i;
				flag = true;
				break;
			}
		}
		if (flag) {
			break;
		}
	}

	// 根据up，down，left，right的宽度选择不同的动画效果
	playExplodeEffect(bomb->getPosition(), upLength, downLength, leftLength, rightLength);
	//
	for (auto box : destroy) {
		box->removeFromParent();
		EventCustom e("boxDestroy");
		e.setUserData((void*)&(box->getPosition()));
		_eventDispatcher->dispatchEvent(&e);
		boxes.eraseObject(box);
	}
	destroy.clear();
	bomb->removeFromParent();
	state->bombNum += 1;
}

void GameScene::boxDestroyHandle(EventCustom * event)
{
	Vec2 pos = *((Vec2*)(event->getUserData()));
	// 在pos位置上随机放置三种道具
	auto i = random(1, 10);
	if (i > 3) {
		return;
	}
	char propPath[20];
	sprintf(propPath, "prop-%d.png", i);
	auto prop = Sprite::create(propPath);

	auto body = PhysicsBody::createBox(prop->getContentSize(), PhysicsMaterial(0.0f, 0.0f, 0.0f));
	body->setDynamic(false);
	prop->setAnchorPoint(Vec2(0.5, 0.5));
	prop->setScale(0.5, 0.5);
	prop->setPosition(pos);
	
	if (i == 1) {
		prop->setName("power");
	}
	else if (i == 2) {
		prop->setName("num");
	}
	else {
		prop->setName("speed");
	}
	
	prop->runAction(
		RepeatForever::create(
			Sequence::create(
				MoveBy::create(1.0f, Vec2(0, 5)),
				MoveBy::create(1.0f, Vec2(0, -5)),
				nullptr
			)
		)
	);
	body->setContactTestBitmask(0xFFFFFFFF);
	prop->setPhysicsBody(body);
	addChild(prop, 1);
}

void GameScene::gameOverHandle(EventCustom * event)
{
	char* message = (char*)(event->getUserData());
	
	
	winner = string(message);
	if (winner == "P1") {
		P2->removeFromParent();
	}
	else {
		P1->removeFromParent();
	}
	unschedule(schedule_selector(GameScene::updateTime));
	unschedule(schedule_selector(GameScene::updateCustom));
	GameOver();
}

Vec2 GameScene::converCoorToPos(blockCoor b)
{
	return Vec2(origin.x + b.x* blockSize + blockSize / 2, origin.y + b.y * blockSize + blockSize / 2);
}

Vec2 GameScene::GetNextPos(Vec2 now, direction dire)
{
	blockCoor temp = standarPos(now);
	switch (dire) {
	case up:
		temp.y += 1;
		return converCoorToPos(temp);
	case down:
		temp.y -= 1;
		return converCoorToPos(temp);
	case left:
		temp.x -= 1;
		return converCoorToPos(temp);
	case right:
		temp.x += 1;
		return converCoorToPos(temp);
	default:
		return Vec2();
	}
}

playerState& GameScene::getPlayState(Sprite * p)
{
	if (p == P1) {
		return P1State;
	}
	else if(p == P2){
		return P2State;
	}
	else {
		return playerState();
	}
}

void GameScene::playExplodeEffect(Vec2 explodePos, int upLength, int downLength, int leftLength, int rightLength)
{
	auto center = Sprite::create("newBom.png", Rect(0, 45, 45, 45));
	center->setPosition(explodePos);
	addChild(center, 1);
	center->runAction(
		Sequence::create(
			Animate::create(AnimationCache::getInstance()->getAnimation("bombExplodingCenter")),
			CallFunc::create([=] {
				center->removeFromParent();
			}),
			nullptr
		));
	Vec2 temp = explodePos;
	auto upAnimate = Animate::create(AnimationCache::getInstance()->getAnimation("bombExplodingHorizontal"));
	for (int i = 0; i < upLength; i++)
	{
		auto left = Sprite::create("newBom.png", Rect(230, 50, 40, 45));
		temp = GetNextPos(temp, direction::up);
		left->setPosition(temp);
		addChild(left, 1);
		left->runAction(
			Sequence::create(
				upAnimate->clone(),
				CallFunc::create([=] {
					left->removeFromParent();
				}),
				nullptr
			));
	}

	temp = explodePos;
	auto downAnimate = Animate::create(AnimationCache::getInstance()->getAnimation("bombExplodingHorizontal"));
	for (int i = 0; i < downLength; i++)
	{
		auto left = Sprite::create("newBom.png", Rect(230, 50, 40, 45));
		temp = GetNextPos(temp, direction::down);
		left->setPosition(temp);
		addChild(left, 1);
		left->runAction(
			Sequence::create(
				downAnimate->clone(),
				CallFunc::create([=] {
					left->removeFromParent();
				}),
				nullptr
			));
	}

	temp = explodePos;
	auto leftAnimate = Animate::create(AnimationCache::getInstance()->getAnimation("bombExplodingVertical"));
	for (int i = 0; i < leftLength; i++)
	{
		auto left = Sprite::create("newBom.png", Rect(120, 50, 45, 31));
		temp = GetNextPos(temp, direction::left);
		left->setPosition(temp);
		addChild(left, 1);
		left->runAction(
			Sequence::create(
				leftAnimate->clone(),
				CallFunc::create([=] {
			left->removeFromParent();
			}),
				nullptr
			));
	}

	temp = explodePos;
	auto rightAnimate = Animate::create(AnimationCache::getInstance()->getAnimation("bombExplodingVertical"));
	for (int i = 0; i < rightLength; i++)
	{
		auto left = Sprite::create("newBom.png", Rect(120, 50, 45, 31));
		temp = GetNextPos(temp, direction::right);
		left->setPosition(temp);
		addChild(left, 1);
		left->runAction(
			Sequence::create(
				rightAnimate->clone(),
				CallFunc::create([=] {
			left->removeFromParent();
		}),
				nullptr
			));
	}
}


