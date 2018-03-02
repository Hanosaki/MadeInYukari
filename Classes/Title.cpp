#pragma execution_character_set("utf-8")
#include "Title.h"
#include "SimpleAudioEngine.h"
#include "StringResouce.h"
#include "GenericFunction.h"

using namespace CocosDenshion;
USING_NS_CC;

Scene* Title::createScene()
{
	auto scene = Scene::create();
	auto layer = Title::create();
	scene->addChild(layer);
	return scene;
}

bool Title::init()
{
	if (!Layer::init())
		return false;

#pragma region 変数宣言
	auto directer = Director::getInstance();
	auto visibleSize = directer->getVisibleSize();
	auto origin = directer->getVisibleOrigin();
	const auto BUTTON_SIZE = Size(visibleSize.width + origin.x, visibleSize.height / 10 + origin.y);
#pragma endregion

	auto audioEngine = SimpleAudioEngine::getInstance();

	//BGM設定
	audioEngine->setBackgroundMusicVolume(0.4f);
	// SE設定
	audioEngine->setEffectsVolume(0.8f);

#pragma region タイトル表記
	auto titleText = FileUtils::getInstance()->getStringFromFile(TEXT_FOLDER + TITLE_TEXT + TXT);
	auto titleLabel = Label::createWithTTF(titleText, FONTS_FOLDER + JPN_FONT, 24);
	titleLabel->setScale(directer->getContentScaleFactor());
	titleLabel->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - titleLabel->getContentSize().height*2);
	titleLabel->setColor(Color3B::BLACK);
	this->addChild(titleLabel, 4);
#pragma endregion

#pragma region 背景設定
	auto backGround = Sprite::create(IMAGE_FOLDER + ROOM + PNG);
	backGround->setContentSize(directer->getVisibleSize());
	backGround->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
	this->addChild(backGround, 1);
#pragma endregion

	auto genericFunc = new GenericFunc;

#pragma region 主人公(立ち絵)の初期設定
	auto characterImage = genericFunc->setMainCharacterImage(visibleSize, origin, IMAGE_FOLDER + YUKARI + PNG);
	characterImage->setTag(2);
	this->addChild(characterImage, 3);
#pragma endregion

#pragma region 終了ボタン配置
	auto endButton = Sprite::create(IMAGE_FOLDER + END + PNG);
	endButton->setContentSize(BUTTON_SIZE);

	auto selectedEndButton = Sprite::create(IMAGE_FOLDER + END + PNG);
	selectedEndButton->setOpacity(128);
	selectedEndButton->setContentSize(BUTTON_SIZE);

	auto endItem = MenuItemSprite::create(endButton, selectedEndButton, CC_CALLBACK_1(Title::closeGame, this));
	auto endMenu = Menu::create(endItem, NULL);
	endMenu->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	endMenu->setPosition(Vec2(origin.x + visibleSize.width / 2, endButton->getContentSize().height/2));

	this->addChild(endMenu, 2);
#pragma endregion

#pragma region クレジットボタン配置
	auto creditButton = Sprite::create(IMAGE_FOLDER + CREDIT + PNG);
	creditButton->setContentSize(BUTTON_SIZE);

	auto selectedCreditButton = Sprite::create(IMAGE_FOLDER + CREDIT + PNG);
	selectedCreditButton->setOpacity(128);
	selectedCreditButton->setContentSize(BUTTON_SIZE);

	auto creditItem = MenuItemSprite::create(creditButton, selectedCreditButton, CC_CALLBACK_1(Title::callCreditScene, this));
	auto creditMenu = Menu::create(creditItem, NULL);
	creditMenu->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	creditMenu->setPositionX(endMenu->getPositionX());
	creditMenu->setPositionY(endMenu->getPositionY() + creditButton->getContentSize().height);

	this->addChild(creditMenu, 2);
#pragma endregion

#pragma region スタートボタン配置
	auto startButton = Sprite::create(IMAGE_FOLDER + START + PNG);
	startButton->setContentSize(BUTTON_SIZE);

	auto selectedStartButton = Sprite::create(IMAGE_FOLDER + START + PNG);
	selectedStartButton->setOpacity(128);
	selectedStartButton->setContentSize(BUTTON_SIZE);

	auto startItem = MenuItemSprite::create(startButton, selectedStartButton, CC_CALLBACK_1(Title::callOPScene, this));
	auto startMenu = Menu::create(startItem, NULL);
	startMenu->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	startMenu->setPositionX(creditMenu->getPositionX());
	startMenu->setPositionY(creditMenu->getPositionY() + startButton->getContentSize().height);

	this->addChild(startMenu, 2);
#pragma endregion

	//Converter converter;
	//auto bgmName = converter.replaceString2Char(F_BGM + TITLE_BGM + TYPE_MP3);
	//SimpleAudioEngine::getInstance()->playBackgroundMusic(bgmName, true);

	return true;
}

void Title::characterImageChange()
{
	//auto characterImage = (Sprite*)this->getChildByTag(2);
	//characterImage->setTexture(F_IMAGE + F_MAIN_CHARACTER + LAUGH);
}

void Title::callOPScene(Ref* Sender)
{
	/*Converter converter;
	SimpleAudioEngine::getInstance()
		->playEffect(converter.replaceString2Char(F_SE + START_VOICE + TYPE_MP3));
	characterImageChange();
	Director::getInstance()->replaceScene(TransitionFade::create(3.0f, Introduction::createScene(), Color3B::BLACK));*/
}

void Title::callCreditScene(Ref* Sender)
{
	//Converter converter;
	//auto seName = converter.replaceString2Char(F_SE + BUTTON_SE + TYPE_MP3);
	//SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	//SimpleAudioEngine::getInstance()->playEffect(seName);
	//characterImageChange();
	//Director::getInstance()->replaceScene(TransitionFade::create(2.0f, Credit::createScene(), Color3B::WHITE));
}

void Title::closeGame(Ref* sender)
{
	SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	Director::getInstance()->purgeCachedData();//キャッシュ開放
	Director::getInstance()->end();
}