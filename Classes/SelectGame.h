#ifndef __SELECT_GAME_H__
#define __SELECT_GAME_H__

#include "cocos2d.h"

class selectGame : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();
	
	bool onTouchBegan(cocos2d::Touch* touch,cocos2d::Event* event);

	void countTimer(float dt);

	void succces();
	void failed();

	CREATE_FUNC(selectGame);

	int num,limit;
	bool isNnoTouch;
	cocos2d::Size visibleSize;
	cocos2d::Vec2 origin;
};

#endif