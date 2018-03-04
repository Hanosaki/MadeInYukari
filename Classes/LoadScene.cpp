#include "LoadScene.h"
#include "Title.h"
#include "SimpleAudioEngine.h"
#include "StringResouce.h"
#include "Converter.h"
#include "AudioEngine.h"

USING_NS_CC;

using namespace CocosDenshion;
using namespace experimental;

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

	srand((unsigned int)time(NULL));//—”‰Šú‰»

	auto con = new Converter;
	auto se = SE_FOLDER + CURTAIN_SE + MP3;
	seId = AudioEngine::play2d(se,false,0.5,nullptr);
	AudioEngine::setFinishCallback(seId,CC_CALLBACK_0(LoadScene::selectGame,this));

}

void LoadScene::update(float dt)
{
	auto seState = AudioEngine::getState(seId);
	if (seState == AudioEngine::AudioState::ERROR)
	{
		selectGame();
	}
}

void LoadScene::selectGame()
{
	int num = rand() % 2;
	Scene* scene;
	switch (num)
	{
	case 0:
		scene = Title::createScene();
		break;
	case 1:
		scene = Title::createScene();
		break;
	default:
		break;
	}
	callGamePart(scene);
}

void LoadScene::callGamePart(Scene* scene)
{
	Director::getInstance()->replaceScene(scene);
}

void LoadScene::loadResouce()
{

}