#pragma execution_character_set("utf-8")
#include "GenericFunction.h"
#include "StringResouce.h"
#include "Converter.h"

USING_NS_CC;

Sprite* GenericFunc::setMainCharacterImage(Size visibleSize, Vec2 origin,std::string imgPath)
{
	auto characterImage = Sprite::create(imgPath);
	characterImage->setScale((origin.y + visibleSize.height) / (characterImage->getContentSize().height));
	characterImage->setPosition(origin.x + visibleSize.width - (characterImage->getContentSize().width / 4 * characterImage->getScale())
		, origin.y + visibleSize.height / 2);
	return characterImage;
}

Sprite* GenericFunc::createSprite(std::string fileName, float x, float y , int tag)
{
	auto scale = Director::getInstance()->getContentScaleFactor();
	auto sprite = Sprite::create(fileName);
	sprite->setPositionX(x);
	sprite->setPositionY(y);
	sprite->setTag(tag);
	sprite->setScale(scale);
	return sprite;
}

Sprite* GenericFunc::createSprite(std::string fileName, float x, float y, Vec2 anchor, int tag)
{
	auto scale = Director::getInstance()->getContentScaleFactor();
	auto sprite = Sprite::create(fileName);
	sprite->setAnchorPoint(anchor);
	sprite->setPositionX(x);
	sprite->setPositionY(y);
	sprite->setTag(tag);
	sprite->setScale(scale);
	return sprite;
}

Sprite* GenericFunc::createSprite(std::string fileName, float x, float y, Vec2 anchor  ,float scale, int tag )
{
	auto sprite = Sprite::create(fileName);
	sprite->setAnchorPoint(anchor);
	sprite->setPositionX(x);
	sprite->setPositionY(y);
	sprite->setTag(tag);
	sprite->setScale(scale);
	return sprite;
}

Sprite* GenericFunc::createSpriteWithRect(Rect rect, float x, float y, Vec2 anchor, Color3B color, int tag)
{
	auto sprite = Sprite::create();
	sprite->setTextureRect(rect);
	sprite->setAnchorPoint(anchor);
	sprite->setPositionX(x);
	sprite->setPositionY(y);
	sprite->setColor(color);
	sprite->setTag(tag);
	return sprite;
}

Vec2 GenericFunc::setWindowCenter(Size visibleSize, Vec2 origin)
{
	return Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y);
}

void GenericFunc::setCurtain(Sprite* curtain[4])
{
	auto director = Director::getInstance();
	auto visibleSize = director->getVisibleSize();
	auto origin = director->getVisibleOrigin();
	for (int i = 0; i < 4; ++i)
	{
		curtain[i] = Sprite::create(IMAGE_FOLDER + CURTAIN + PNG);
		auto scale = visibleSize.width / 4 / curtain[i]->getContentSize().width;
		curtain[i]->setScale(scale);
		curtain[i]->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
		curtain[i]->setPosition(
			curtain[i]->getContentSize().width * scale * i,
			origin.y + visibleSize.height / 2);
		curtain[i]->setTag(1 + i);
	}
}

void GenericFunc::setOpendCurtain(Sprite* curtain[4])
{
	auto director = Director::getInstance();
	auto visibleSize = director->getVisibleSize();
	auto origin = director->getVisibleOrigin();
	for (int i = 0; i < 4; ++i)
	{
		if (i < 2)
		{
			curtain[i] = Sprite::create(IMAGE_FOLDER + CURTAIN + PNG);
			auto scale = visibleSize.width / 4 / curtain[i]->getContentSize().width;
			curtain[i]->setScale(scale);
			curtain[i]->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
			curtain[i]->setPosition(
				0-curtain[i]->getContentSize().width * scale,
				origin.y + visibleSize.height / 2);
		}
		else
		{
			curtain[i] = Sprite::create(IMAGE_FOLDER + CURTAIN + PNG);
			auto scale = visibleSize.width / 4 / curtain[i]->getContentSize().width;
			curtain[i]->setScale(scale);
			curtain[i]->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
			curtain[i]->setPosition(
				origin.x + visibleSize.width,
				origin.y + visibleSize.height / 2);

		}
		curtain[i]->setTag(1 + i);
	}
}