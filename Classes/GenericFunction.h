#ifndef __GENERIC_FUNCTION_H__
#define __GENERIC_FUNCTION_H__

#include "cocos2d.h"
class GenericFunc{
public:
	cocos2d::Sprite*  setMainCharacterImage(cocos2d::Size visibleSize, cocos2d::Vec2 origin,std::string imgPath);
	cocos2d::Vec2 setWindowCenter(cocos2d::Size visibleSize,cocos2d::Vec2 origin);
	cocos2d::Sprite* createSprite(std::string fileName, float x, float y,int tag);
	cocos2d::Sprite* createSprite(std::string fileName, float x, float y,cocos2d::Vec2 anchor , int tag);
	cocos2d::Sprite* createSprite(std::string fileName, float x, float y, cocos2d::Vec2 anchor, float scale, int tag);
	cocos2d::Sprite* createSpriteWithRect(cocos2d::Rect rect, float x, float y, cocos2d::Vec2 anchor, cocos2d::Color3B color, int tag);
	void setCurtain(cocos2d::Sprite* curtain[4]);
	void setOpendCurtain(cocos2d::Sprite* curtain[4]);

};

#endif