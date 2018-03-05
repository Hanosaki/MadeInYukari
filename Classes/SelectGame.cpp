#include "SelectGame.h"
#include "StringResouce.h"
#include "GenericFunction.h"
#include "LoadScene.h"
#include "AudioEngine.h"

USING_NS_CC;

using namespace experimental;

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
	visibleSize = director->getVisibleSize();
	origin = director->getVisibleOrigin();
	isNnoTouch = true;
	isCleared = false;
	limit = 6;

	auto generilFunc = new GenericFunc;

	auto backGroundRect = Rect(0, 0, origin.x + visibleSize.width, origin.y + visibleSize.height);
	auto backGround = generilFunc->createSpriteWithRect(backGroundRect, 0, 0, Vec2::ANCHOR_BOTTOM_LEFT, Color3B::WHITE, 0);
	this->addChild(backGround);

	Sprite* question;
	srand((unsigned int)time(NULL));
	num = rand() % 3;
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

	auto listner = EventListenerTouchOneByOne::create();
	listner->onTouchBegan = CC_CALLBACK_2(selectGame::onTouchBegan, this);
	director->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listner, this);

	this->schedule(schedule_selector(selectGame::countTimer), 1.0f);

	auto timeLabel = Label::createWithTTF(StringUtils::toString(limit), FONTS_FOLDER + ENG_FONT, 128);
	timeLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	timeLabel->setPosition(origin.x + visibleSize.width/3, origin.y + 3*visibleSize.height/4);
	timeLabel->setTag(4);
	timeLabel->setColor(Color3B::BLACK);
	this->addChild(timeLabel);
	return true;

}

bool selectGame::onTouchBegan(Touch* touch, Event* event)
{
	auto leftBox = this->getChildByTag(1)->getBoundingBox();
	auto middleBox = this->getChildByTag(2)->getBoundingBox();
	auto rightBox = this->getChildByTag(3)->getBoundingBox();
	auto location = touch->getLocation();

	if (location.y <= origin.y + visibleSize.height / 2 && isNnoTouch)
	{
		isNnoTouch = false;
		#pragma region 判定
		switch (num)
		{
		case 0:
			if (leftBox.containsPoint(location))
			{
				succces();
			}
			else
			{
				failed();
			}
			break;
		case 1:
			if (middleBox.containsPoint(location))
			{
				succces();
			}
			else
			{
				failed();
			}
			break;
		case 2:
			if (rightBox.containsPoint(location))
			{
				succces();
			}
			else
			{
				failed();
			}
			break;
		default:
			break;
		}
		#pragma endregion
	}

	return true;

}

void selectGame::succces()
{
	AudioEngine::play2d(SE_FOLDER + CLEAR_SE + MP3, false, 0.5f, nullptr);
	Sprite* curtain[4];
	auto genericFunc = new GenericFunc;
	genericFunc->setOpendCurtain(curtain);
	addCurtains(curtain);
	genericFunc->moveCurtainClose(curtain);
	isCleared = true;
}

void selectGame::failed()
{
	AudioEngine::play2d(SE_FOLDER + MISS_SE + MP3, false, 0.5f, nullptr);
	Sprite* curtain[4];
	auto genericFunc = new GenericFunc;
	genericFunc->setOpendCurtain(curtain);
	addCurtains(curtain);
	genericFunc->moveCurtainClose(curtain);
	if (isNnoTouch)
	{
		isNnoTouch = false;
		this->scheduleOnce(schedule_selector(selectGame::failedEnd), 2.0f);
	}


}

void selectGame::addCurtains(Sprite* curtain[4])
{
	for each (auto item in curtain)
	{
		this->addChild(item);
	}
}

void selectGame::countTimer(float dt)
{
	if (limit > 0)
	{
		--limit;
		auto timeLabel = (Label*)this->getChildByTag(4);
		timeLabel->setString(StringUtils::toString(limit));
	}
	else
	{
		if (isCleared)
			Director::getInstance()->replaceScene(LoadScene::createScene());
		else if(!isNnoTouch)
			Director::getInstance()->replaceScene(LoadScene::createScene());
		else
			failed();

		this->unschedule(schedule_selector(selectGame::countTimer));
	}
}

void selectGame::failedEnd(float dt)//時間切れなどで失敗した際の処理
{
	Director::getInstance()->replaceScene(LoadScene::createScene());
}