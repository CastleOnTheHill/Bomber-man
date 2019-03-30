#include "MenuScene.h"
#include "SimpleAudioEngine.h"
#include "GameScene.h"
#include "RankScene.h"
#include "DesScene.h"

USING_NS_CC;

using namespace CocosDenshion;

Scene* MenuScene::createScene()
{
    return MenuScene::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool MenuScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }
		UserDefault::getInstance()->setStringForKey("notExist", "Not Exist Yet");
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto bg = Sprite::create("bg1.png");
	bg->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
	bg->setContentSize(Size(visibleSize.width, visibleSize.height));
	this->addChild(bg, 0);

	preloadMusic();

	SimpleAudioEngine::getInstance()->playBackgroundMusic("menuBgm.mp3", true);

	ValueMap p_map = FileUtils::getInstance()->getValueMapFromFile("menu.xml");
	auto start = p_map.at("start").asString();
	auto rank = p_map.at("rank").asString();
	auto des = p_map.at("description").asString();

	auto closeItem = MenuItemImage::create(
		"CloseNormal.png", 
		"CloseSelected.png", 
		CC_CALLBACK_1(MenuScene::menuCloseCallback, this));

	if (closeItem == nullptr ||
		closeItem->getContentSize().width <= 0 ||
		closeItem->getContentSize().height <= 0) {
		problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
	}
	else {
		float x = origin.x + visibleSize.width - closeItem->getContentSize().width / 2;
		float y = origin.y + closeItem->getContentSize().height / 2;
		closeItem->setPosition(Vec2(x, y));
	}


	auto gotoGameScene = MenuItemLabel::create(Label::createWithTTF(start, 
		"fonts/simsun.ttc", 48), 
		[](Ref* pSender) {
		Director::getInstance()->pushScene(GameScene::createScene()); 
		SimpleAudioEngine::getInstance()->playEffect("menuClick.mp3", false);
	});
	gotoGameScene->setColor(Color3B(0, 0, 0));
	if (gotoGameScene) {
		float x = origin.x + visibleSize.width / 2;
		float y = origin.y + visibleSize.height - 300;
		gotoGameScene->setPosition(Vec2(x, y));
	}

	auto gotoRankScene = MenuItemLabel::create(Label::createWithTTF(rank,
		"fonts/simsun.ttc", 48),
		[](Ref* pSender) {
		Director::getInstance()->pushScene(RankScene::createScene());
		SimpleAudioEngine::getInstance()->playEffect("menuClick.mp3", false);
	});
	gotoRankScene->setColor(Color3B(0, 0, 0));
	if (gotoRankScene) {
		float x = origin.x + visibleSize.width / 2;
		float y = origin.y + visibleSize.height - 400;
		gotoRankScene->setPosition(Vec2(x, y));
	}

	auto gotoDescriptionScene = MenuItemLabel::create(Label::createWithTTF(des,
		"fonts/simsun.ttc", 48),
		[](Ref* pSender) {
		Director::getInstance()->pushScene(DesScene::createScene());
		SimpleAudioEngine::getInstance()->playEffect("menuClick.mp3", false);
	});
	gotoDescriptionScene->setColor(Color3B(0, 0, 0));
	if (gotoDescriptionScene) {
		float x = origin.x + visibleSize.width / 2;
		float y = origin.y + visibleSize.height - 500;
		gotoDescriptionScene->setPosition(Vec2(x, y));
	}

	auto menu = Menu::create(closeItem, gotoGameScene, gotoRankScene, gotoDescriptionScene, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

    return true;
}

void MenuScene::preloadMusic() {
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic("menuBgm.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("menuClick.mp3");
}

void MenuScene::menuCloseCallback(Ref* pSender) {
	//Close the cocos2d-x game scene and quit the application
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif

	/*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/

	//EventCustom customEndEvent("game_scene_close_event");
	//_eventDispatcher->dispatchEvent(&customEndEvent);


}