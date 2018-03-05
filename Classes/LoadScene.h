#ifndef __LAOD_SCENE_H__
#define __LAOD_SCENE_H__
#include "cocos2d.h"

class LoadScene :public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	CREATE_FUNC(LoadScene);
	
	void loadResouce();
	void callGamePart();
	void selectGame();
	void animation(float dt);

	int seId,gameNumber;

};

#endif
