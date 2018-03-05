#include "SelectGame.h"
#include "StringResouce.h"
#include "GenericFunction.h"

USING_NS_CC;

Scene* selectGame::createScene()
{
	auto scene = Scene::create();
	auto layer = selectGame::create();
	scene->addChild(layer);
	return scene;
}

bool selectGame::init()
{
	if (!Layer::init())
		return false;

	auto director = Director::getInstance();
	auto visibleSize = director->getVisibleSize();
	auto origin = director->getVisibleOrigin();

	auto generilFunc = new GenericFunc;

	auto backGroundRect = Rect(0, 0, origin.x + visibleSize.width, origin.y + visibleSize.height);
	auto backGround = generilFunc->createSpriteWithRect(backGroundRect, 0, 0, Vec2::ANCHOR_BOTTOM_LEFT, Color3B::WHITE, 0);
	this->addChild(backGround);

	Sprite* question;
	srand((unsigned int)time(NULL));
	auto num = rand() % 3;
	std::string hoge;
	switch (num)//お題をランダムで決定
	{
	case 0:
		question = Sprite::create(SELECT_GAME_FOLDER + RUN + PNG); break;
	case 1:
		question = Sprite::create(SELECT_GAME_FOLDER + JUMP + PNG); break;
	case 2:
		question = Sprite::create(SELECT_GAME_FOLDER + DAMAGE + PNG); break;
	default:
		break;
	}

	question->setScale((visibleSize.height / 2) / question->getContentSize().height);
	question->setPosition(origin.x + visibleSize.width /2,origin.y +3 * visibleSize.height/4);
	question->setColor(Color3B::BLACK);
	this->addChild(question);

	/*選択肢用の箱を設置*/
	auto boxRect = Rect(0, 0, origin.x + visibleSize.width / 3, origin.y + visibleSize.height / 2);
	auto leftBox = generilFunc->createSpriteWithRect(boxRect, 0, 0, Vec2::ANCHOR_BOTTOM_LEFT, Color3B::RED, 1);
	leftBox->setOpacity(128);
	this->addChild(leftBox);

	auto middleBox = generilFunc->createSpriteWithRect(boxRect, origin.x + visibleSize.width / 2, 0, Vec2::ANCHOR_MIDDLE_BOTTOM, Color3B::GREEN, 2);
	middleBox->setOpacity(128);
	this->addChild(middleBox);

	auto rightBox = generilFunc->createSpriteWithRect(boxRect, origin.x + visibleSize.width, 0, Vec2::ANCHOR_BOTTOM_RIGHT, Color3B::BLUE, 3);
	rightBox->setOpacity(128);
	this->addChild(rightBox);

	auto ans1 = Sprite::create(SELECT_GAME_FOLDER + RUN + PNG);
	ans1->setScale(question->getScale());
	ans1->setPosition(leftBox->getContentSize() / 2);
	leftBox->addChild(ans1);

	auto ans2 = Sprite::create(SELECT_GAME_FOLDER + JUMP + PNG);
	ans2->setScale(question->getScale());
	ans2->setPosition(middleBox->getContentSize()/2);
	middleBox->addChild(ans2);

	auto ans3 = Sprite::create(SELECT_GAME_FOLDER + DAMAGE + PNG);
	ans3->setScale(question->getScale());
	ans3->setPosition(rightBox->getContentSize() / 2);
	rightBox->addChild(ans3);

	return true;

}