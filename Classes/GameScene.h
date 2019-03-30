#pragma once
#include <stdio.h>
#include "cocos2d.h"

USING_NS_CC;
using std::string;

enum direction
{
	up, down, left, right
};

struct blockCoor{
	int x;
	int y;
	blockCoor(int ele1 = 0, int ele2 = 0) {
		x = ele1;
		y = ele2;
	}
};

struct playerState
{
	int power;
	int speed;
	int bombNum;
	bool canPushBomb;
	playerState() {
		power = 3; //每次加2
		speed = 100;
		bombNum = 1;
		canPushBomb = false;
	}
};

class GameScene : public cocos2d::Layer
{
public:
	PhysicsWorld * m_world;
	void setPhyWorld(PhysicsWorld* world) { m_world = world; };

	static cocos2d::Scene* createScene();

	virtual bool init();


	//virtual void shootMenuCallback(Ref* pSender);

	CREATE_FUNC(GameScene);

	void initMap();
	void initAllEle();
	void updateCustom(float dt);

	void onKeyPressed(EventKeyboard::KeyCode code, Event * event);
	void onKeyReleased(EventKeyboard::KeyCode code, Event * event);
	bool onConcactBegin(PhysicsContact & contact);
	void preloadMusic();
	void GameOver();
	void yesBtnCallback(Ref * pSender);
	void exitCallback(Ref * pSender);
	void replayCallback(Ref * pSender);
	bool PlaceBomb(playerState &state, Vec2 pos);
	void updateTime(float dt);
	void loadAnimation();
	blockCoor standarPos(Vec2 before);
	void addaddListener();
	bool isInMap(Vec2 point);

	void explodeHandle(EventCustom* event);
	void boxDestroyHandle(EventCustom* event);
	void gameOverHandle(EventCustom* event);
	Vec2 converCoorToPos(blockCoor b);
	Vec2 GetNextPos(Vec2 now, direction dire);
	playerState& getPlayState(Sprite* p);

	void playExplodeEffect(Vec2 explodePos, int upLength, int downLength, int leftLength, int rightLength);
private:
	Sprite * P1;
	Sprite* P2;
	playerState P1State;
	playerState P2State;
	TMXTiledMap* map;
	cocos2d::Vector<Sprite*> boxes;
	cocos2d::Vector<Sprite*> walls;
	Size visibleSize;
	float blockSize;
	Vec2 origin;  //可移动地图的左下角
	float scaleSize;
	int mapWidthBlock;
	int mapHeightBlock;
	bool P1IsMoving;
	bool P2IsMoving;
	direction P1MoveDriection;
	direction P2MoveDriection;
	string winner;
};