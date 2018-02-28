﻿#ifndef __SPLASH_H__
#define __SPLASH_H__

#include "cocos2d.h"


class Splash : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(Splash);

	void callLoadScene(float dt);

};

#endif