#pragma execution_character_set("utf-8")
#include "LoadScene.h"
#include "Title.h"
#include "SimpleAudioEngine.h"
#include "StringResouce.h"
#include "Converter.h"
#include "AudioEngine.h"

USING_NS_CC;

using namespace CocosDenshion;
using namespace experimental;

Scene* nextScene(int gameNumber);

Scene* LoadScene::createScene()
{
	auto scene = Scene::create();
	auto layer = LoadScene::create();
	scene->addChild(layer);
	return scene;
}

bool LoadScene::init()
{
	if (!Layer::init())
		return false;

	auto director = Director::getInstance();
	auto visibleSize = director->getVisibleSize();
	auto origin = director->getVisibleOrigin();

	Label* centerLabel = Label::createWithTTF("", FONTS_FOLDER + JPN_FONT, 36);
	centerLabel->setPosition(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2);
	centerLabel->setColor(Color3B::WHITE);
	centerLabel->setTag(5);
	this->addChild(centerLabel);

	selectGame();

	auto con = new Converter;
	auto se = SE_FOLDER + CURTAIN_SE + MP3;
	seId = AudioEngine::play2d(se,false,0.5,nullptr);
	AudioEngine::setFinishCallback(seId,CC_CALLBACK_0(LoadScene::callGamePart,this)); 

	Sprite* curtain[4];//画面カーテン設置
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

		switch (i)//表示優先順位の設定(真ん中の二枚が上になるように)
		{
		case 1:
			this->addChild(curtain[i], 1);
			break;
		case 2:
			this->addChild(curtain[i], 1);
			break;
		default:
			this->addChild(curtain[i]);
			break;
		}
	}
	this->scheduleOnce(schedule_selector(LoadScene::animation), 3.0f);

	return true;

}

void LoadScene::selectGame()
{
	srand((unsigned int)time(NULL));//乱数初期化
	gameNumber = rand() % 2;
	std::string gameTitle;
	auto file = FileUtils::getInstance();
	switch (gameNumber)//次に実行するシーンをランダムで決める(予定)
	{
	case 0:
		gameTitle = file->getStringFromFile("selectGame/title.txt");
		break;
	case 1:
		gameTitle = file->getStringFromFile("selectGame/title.txt");
		break;
	default:
		break;
	}
	auto centerLabel = (Label*)this->getChildByTag(5);
	centerLabel->setString(gameTitle);
}

void LoadScene::animation(float dt)
{
	auto leftCurtain = (Sprite*)this->getChildByTag(2);
	auto leftSeq = Sequence::create(MoveTo::create(1.0f, Vec2(0, leftCurtain->getPositionY())),DelayTime::create(1.0f),ScaleTo::create(25.0f,3.0f),NULL);
	leftCurtain->runAction(leftSeq);
	auto rightCurtain = (Sprite*)this->getChildByTag(3);
	auto rightCurtain2 = (Sprite*)this->getChildByTag(4);
	auto rightSeq = Sequence::create(MoveTo::create(1.0f, rightCurtain2->getPosition()), DelayTime::create(1.0f), ScaleTo::create(25.0f, 3.0f), NULL);
	rightCurtain->runAction(rightSeq);
}

void LoadScene::callGamePart()
{
	Director::getInstance()->replaceScene(TransitionFade::create(1.5f, nextScene(gameNumber),Color3B::WHITE));
}

Scene* nextScene(int gameNumber)
{
	Scene* next;
	switch (gameNumber)
	{
	case 0:
		next = Title::createScene(); break;
	case 1:
		next = Title::createScene(); break;
	default:
		break;
	}
	return next;
}

void LoadScene::loadResouce()
{

}