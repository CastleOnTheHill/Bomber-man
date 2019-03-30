#pragma once
#include <stdio.h>
#include "cocos2d.h"
USING_NS_CC;

class DesScene : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	CREATE_FUNC(DesScene);

private:

};