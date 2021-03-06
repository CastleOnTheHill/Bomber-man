#pragma once
#include <stdio.h>
#include "cocos2d.h"
USING_NS_CC;

class RankScene : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	CREATE_FUNC(RankScene);

private:
	std::string name;
	int score;
};
