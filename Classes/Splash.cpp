#pragma execution_character_set("utf-8")
#include "Splash.h"
#include "SimpleAudioEngine.h"
#include "HelloWorldScene.h"

using namespace CocosDenshion;
USING_NS_CC;

Scene* Splash::createScene()
{
	auto scene = Scene::create();
	auto layer = Splash::create();
	scene->addChild(layer);
	return scene;
}

bool Splash::init()
{
	if (!Layer::init())
		return false;

	auto director = Director::getInstance();
	auto visibleSize = director->getVisibleSize();
	auto origin = director->getVisibleOrigin();
	char* seName = "se/Logo.mp3";
	
	SimpleAudioEngine::getInstance()->preloadEffect(seName);

	auto logoText = FileUtils::getInstance()->getStringFromFile("text/logo.txt");

	auto logo = Label::createWithTTF(logoText,"fonts/HGRPP1.TTC", 18);
	logo->setTextColor(Color4B::WHITE);
	logo->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
	logo->setOpacity(0);
	logo->setScale(director->getContentScaleFactor());
	this->addChild(logo);

	auto logoAction = Sequence::create(FadeIn::create(1.0f), DelayTime::create(2.0f), FadeOut::create(1.0f), NULL);
	logo->runAction(logoAction);

	SimpleAudioEngine::getInstance()->playEffect(seName);

	this->scheduleOnce(schedule_selector(Splash::callLoadScene), 5.0f);

	return true;

}

void Splash::callLoadScene(float dt)
{
	Director::getInstance()->replaceScene(HelloWorld::createScene());
}

