#ifndef __TITLE_H__
#define __TITLE_H__

#include "cocos2d.h"

class Title : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	CREATE_FUNC(Title);

	void characterImageChange();
	void callOPScene(cocos2d::Ref* sender);
	void callCreditScene(cocos2d::Ref* sender);
	void closeGame(cocos2d::Ref* sender);
	

};

#endif