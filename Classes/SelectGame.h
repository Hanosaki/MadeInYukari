#ifndef __SELECT_GAME_H__
#define __SELECT_GAME_H__

#include "cocos2d.h"

class selectGame : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	CREATE_FUNC(selectGame);
};

#endif