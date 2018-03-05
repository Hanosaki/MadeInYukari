#pragma execution_character_set("utf-8")
#include "GameScene.h"
#include "GenericFunction.h"
#include "SimpleAudioEngine.h"
#include "Converter.h"

using namespace CocosDenshion;
USING_NS_CC;

void nextScene();
float setEnemySpeed();

const int Game::MAX_LIFE = 3;

Scene* Game::createScene()
{
	auto scene = Scene::create();
	auto layer = Game::create();
	scene->addChild(layer);
	return scene;
}

bool Game::init()
{
	if (!Layer::init())
	{
		return false;
	}

#pragma region グローバル変数の初期化
	auto director = Director::getInstance();
	visibleSize = director->getVisibleSize();
	origin = director->getVisibleOrigin();
	scaleFactor = director->getContentScaleFactor();
	score = 0;
	hitCounter = 0;
	animeSpeed = 0.04f;
	enemySpeed = 1.2f;
	endFlag = false;
	hitOnlyOne = false;
	jumpFlag = false;
	speedChangeFlag = false;
	isPause = false;
	nextSceneFlag = false;
	defoultPos = Vec2(visibleSize.width / 6 + origin.x, origin.y);
	enemyDefaultPos = Vec2(2.5f * visibleSize.width + origin.x, visibleSize.height / 6 + origin.y);
	outOfWindowBGPos = Vec2(3 * visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y);
#pragma endregion

	GenericFunc genericFunc;

	/*音量設定*/
	SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.5f);
	SimpleAudioEngine::getInstance()->setEffectsVolume(1.0f);

#pragma region 背景初期設定
	auto backGround = genericFunc.createSprite(F_IMAGE + F_UI + BACK_GROUND,
		origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2, 51);
	backGround->setContentSize((Size)Vec2(visibleSize.width + 0.1f*visibleSize.width, visibleSize.height));
	this->addChild(backGround);

	auto backGround2 = genericFunc.createSprite(F_IMAGE + F_UI + BACK_GROUND,
		outOfWindowBGPos.x, outOfWindowBGPos.y, 52);
	backGround2->setContentSize(backGround->getContentSize());
	this->addChild(backGround2);
#pragma endregion

#pragma region 主人公(animation)スプライトの初期設定
	auto mainCharacter = genericFunc.createSprite(F_IMAGE + F_ANIMATION + F_RUN + DEFAULT,
		defoultPos.x, defoultPos.y, Vec2::ANCHOR_MIDDLE_BOTTOM, 1);
	mainCharacter->setScale((origin.y + visibleSize.height) / (mainCharacter->getContentSize().height * 2));
	mainCharacter->setFlippedX(true);
	this->addChild(mainCharacter, 1);
#pragma endregion

	setCharacterDefault();

#pragma region ポーズボタン

	auto pauseButton = Sprite::create(F_IMAGE + F_UI + PAUSE_BUTTON);
	pauseButton->setTag(41);
	auto selectedPauseButton = Sprite::create(F_IMAGE + F_UI + PAUSE_BUTTON);
	selectedPauseButton->setOpacity(128);
	selectedPauseButton->setTag(42);

	auto pauseItem = MenuItemSprite::create(pauseButton, selectedPauseButton, CC_CALLBACK_1(Game::pauseGame, this));
	pauseItem->setScale(scaleFactor - 0.5*scaleFactor);
	pauseItem->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
	pauseItem->setPosition(origin.x + visibleSize.width, origin.y + visibleSize.height);
	pauseItem->setTag(43);
	auto pauseMenu = Menu::create(pauseItem, NULL);
	pauseMenu->setPosition(Vec2::ZERO);
	pauseMenu->setTag(44);
	this->addChild(pauseMenu, 2);

#pragma endregion

#pragma region 主人公当たり判定
	auto hitDeterminationBox = Rect(0, 0,
		mainCharacter->getContentSize().width / 5.5f * mainCharacter->getScaleX(),
		mainCharacter->getContentSize().height / 2.5f * mainCharacter->getScaleY());
	auto hitDetermination = genericFunc.createSpriteWithRect(hitDeterminationBox,
		mainCharacter->getPositionX(), 0, Vec2::ANCHOR_MIDDLE_BOTTOM, Color3B::BLACK, 11);
	hitDetermination->setVisible(false);
	this->addChild(hitDetermination);
#pragma endregion

#pragma region 主人公(立ち絵)の初期設定
	auto characterImage = Sprite::create(F_IMAGE + F_MAIN_CHARACTER + FACE_AWARENESS);
	characterImage = genericFunc.createSprite(F_IMAGE + F_MAIN_CHARACTER + FACE_AWARENESS,
		0, origin.y + visibleSize.height, Vec2::ANCHOR_TOP_LEFT, scaleFactor - 0.6*scaleFactor, 2);
	this->addChild(characterImage, 3);
#pragma endregion

#pragma region 体力ゲージの初期設定
	Sprite* life[MAX_LIFE];
	auto lifeIcon = Sprite::create(F_IMAGE + F_UI + LIFE_ICON);
	auto lifePostionBase = Vec2(2 * characterImage->getContentSize().width/3 * characterImage->getScale(),
		characterImage->getContentSize().height * characterImage->getScale());
	for (int i = 0; i < MAX_LIFE; ++i)
	{
		auto scale = scaleFactor - 0.65*scaleFactor;
		life[i] = genericFunc.createSprite(F_IMAGE + F_UI + LIFE_ICON,
			origin.x + lifePostionBase.x + lifeIcon->getContentSize().width * scale * (MAX_LIFE - i),
			origin.y + visibleSize.height - lifePostionBase.y / 2, Vec2::ANCHOR_MIDDLE, scale, 20 + i);
		auto rollSpeed = 0.5f;
		auto angle = 25;
		auto rollLeft = RotateTo::create(rollSpeed, angle);
		auto rollRight = RotateTo::create(rollSpeed, -angle);
		auto sequence = Sequence::create(rollLeft, rollRight, NULL);
		life[i]->runAction(RepeatForever::create(sequence));
		this->addChild(life[i], 3);
	}

#pragma endregion

#pragma region 敵の初期設定
	auto enemy = genericFunc.createSprite(F_IMAGE + F_RIVAL + ENEMY_IMAGE,
		 enemyDefaultPos.x*1.25f, enemyDefaultPos.y, Vec2::ANCHOR_MIDDLE_RIGHT,
		 scaleFactor + 0.75*scaleFactor, 31);
	this->addChild(enemy, 1);
#pragma endregion

#pragma region 予測線の生成
	auto noticeRect = Rect(0, 0, visibleSize.width + origin.x,
		enemy->getContentSize().height * enemy->getScale());
	auto noticeLine = genericFunc.createSpriteWithRect(noticeRect,
		origin.x + visibleSize.width / 2, enemy->getPositionY(),
		Vec2::ANCHOR_MIDDLE, Color3B(255, 182, 193), 32);
	auto sequence = Sequence::create(FadeOut::create(0.3f), FadeIn::create(0.5f), NULL);
	noticeLine->runAction(RepeatForever::create(sequence));
	noticeLine->setTag(32);
	this->addChild(noticeLine, 0);
#pragma endregion

#pragma region クリックリスナーの初期設定
	auto listner = EventListenerTouchOneByOne::create();
	listner->onTouchBegan = CC_CALLBACK_2(Game::onTouchBegan, this);
	listner->onTouchEnded = CC_CALLBACK_2(Game::onTouchEnded, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listner, this);
#pragma endregion

	// BGM再生
	Converter converter;
	auto bgmName = converter.replaceString2Char(F_BGM + MAIN_BGM + TYPE_MP3);
	SimpleAudioEngine::getInstance()->playBackgroundMusic(bgmName, true);

	//SE再生
	auto seName = converter.replaceString2Char(F_SE + RIVAL_VOICE + TYPE_MP3);
	SimpleAudioEngine::getInstance()->playEffect(seName);

#pragma region 繰り返し処理の初期設定
	//ゲームメイン処理
	this->runAction(Sequence::create(DelayTime::create(1.5f), CallFunc::create([this]()
	{
		this->schedule(schedule_selector(Game::main), Parameter::GAME_SPEED); 
	}), NULL));
	
	this->schedule(schedule_selector(Game::moveCharacters), Parameter::GAME_SPEED);//主人公のジャンプモーション
	
#pragma endregion

	return true;
}

bool Game::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{

#pragma region ジャンプ開始
	if (!endFlag)
	{
		#pragma region 初回プレイ時の処理
		if (isFirstPlay)
		{
			if (!this->isScheduled(schedule_selector(Game::main)))
			{
				isFirstPlay = false;
				jump();
				this->schedule(schedule_selector(Game::main), Parameter::GAME_SPEED);
				this->removeChildByTag(53);
				this->removeChildByTag(54);
			}
		}
		#pragma endregion
		else if (!jumpFlag && !hitOnlyOne)
		{
		#pragma region 通常プレイ時の処理
			auto mainCharacter = (Sprite*)this->getChildByTag(1);
			if (mainCharacter->isRunning())
			{
				jump();
			}
		#pragma endregion
		}
		
			
	}
	else if (nextSceneFlag)
	{
		Director::getInstance()->replaceScene(GameOver::createScene());
	}
#pragma endregion

	return true;
}

void Game::main(float dt)
{
#pragma region 変数の宣言
	auto mainCharacter = (Sprite*)this->getChildByTag(1);
	auto move = Parameter::MOVE_SPEED;
	auto backGround = this->getChildByTag(51);
	auto backGround2 = this->getChildByTag(52);
	auto enemy = this->getChildByTag(31);
	auto enemyPos = enemy->getPosition();
#pragma endregion

#pragma region 背景アニメーション
	auto pos = backGround->getPosition().x;
	auto pos2 = backGround2->getPosition().x;
	if (!hitOnlyOne){
		pos -= move;
		pos2 -= move;
	}

	if (pos + backGround->getContentSize().width / 2 < 0){
		pos = outOfWindowBGPos.x;
	}
	if (pos2 + backGround2->getContentSize().width / 2 <0){
		pos2 = outOfWindowBGPos.x;
	}
	backGround->setPositionX(pos);
	backGround2->setPositionX(pos2);
#pragma endregion

#pragma region 初回プレイ時のチュートリアル呼び出し
	if (isFirstPlay)
	{
		auto distance = mainCharacter->getPositionX() - enemy->getPositionX();
		if (distance >= -780)
			tutorial();
	}
#pragma endregion

#pragma region ゲームオーバーフラグの起動
	if (hitCounter >= MAX_LIFE && !endFlag)
		endFlag = !endFlag;
#pragma endregion

	if (!endFlag)
	{

	#pragma region エネミーの移動
		auto noticeLine = (Sprite*)this->getChildByTag(32);//予告線スプライトの取得
		enemyPos.x -= 2 * move*enemySpeed;
		enemy->setPosition(enemyPos);
		if (enemy->getPositionX() - (enemy->getContentSize().width * enemy->getScale()) / 2 > visibleSize.width)
			noticeLine->setPositionY(enemy->getPositionY());
		else if (noticeLine->isVisible())
		{
			noticeLine->setVisible(false);
		}

		if (enemyPos.x + enemy->getContentSize().width < 0
			&& enemy->isVisible()
			&& mainCharacter->getPositionY() == defoultPos.y)
		{
			++score;
			enemy->setVisible(false);//画面外に消えたら非表示に
			this->scheduleOnce(schedule_selector(Game::enemyResporn), 0.5f);//敵の再生成
			hitOnlyOne = false;
		}

	#pragma endregion

	#pragma region 接触判定
		auto hitDetermination = (Sprite*)this->getChildByTag(11);
		hitDetermination->setPositionY(mainCharacter->getPositionY()
			+ mainCharacter->getContentSize().height / 4
			* mainCharacter->getScale());

		auto rectMainCharactor = hitDetermination->getBoundingBox();
		auto rectEnemy = enemy->getBoundingBox();

		if (rectMainCharactor.intersectsRect(rectEnemy) && !hitOnlyOne)
		{
			jumpPower = 0;
			Converter converter;
			auto seName = converter.replaceString2Char(F_SE + DAMEGE_VOICE + TYPE_MP3);
			SimpleAudioEngine::getInstance()->stopAllEffects();
			SimpleAudioEngine::getInstance()->playEffect(seName);
			auto characterImage = (Sprite*)this->getChildByTag(2);
			mainCharacter->stopActionByTag(101);
			mainCharacter->setTexture(F_IMAGE + F_MAIN_CHARACTER + DAMAGE_ANIME);
			characterImage->setTexture(F_IMAGE + F_MAIN_CHARACTER + FACE_DAMAGE);
			this->getChildByTag(20 + hitCounter)->setVisible(false);
			hitOnlyOne = true;
			++hitCounter;

		}
	#pragma endregion

	}
	else
	{

#pragma region ゲームオーバー時の処理

		if (SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying())
		{
			SimpleAudioEngine::getInstance()->stopBackgroundMusic();
			auto menu = (Menu*)this->getChildByTag(44);
			menu->setEnabled(false);
			menu->setOpacity(false);
			mainCharacter->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			float rotateSpeed = 0.7f - score / 100;
			mainCharacter->runAction(RepeatForever::create(RotateBy::create(rotateSpeed, -360.f*5)));

		}
		enemyPos.x -= 2 * move*enemySpeed;
		enemy->setPosition(enemyPos);
		auto mainCharacterPos = mainCharacter->getPosition();
		if (score < 20)
			mainCharacter->setPositionX(mainCharacterPos.x - move * 3);
		else
			mainCharacter->setPositionX(mainCharacterPos.x - move * enemySpeed *2);
		mainCharacter->setPositionY(mainCharacterPos.y + move*4);

		if (enemyPos.x + enemy->getContentSize().width < 0 && 
			mainCharacter->getPositionX() < -mainCharacter->getContentSize().width/2)
		{
			auto gameoverLabel = Label::createWithTTF(GAME_OVER_TEXT, F_FONTS + JPN_FONTS, Parameter::LARGE * scaleFactor);
			gameoverLabel->setPosition(visibleSize.width / 2, visibleSize.height / 2);
			this->addChild(gameoverLabel, 4);
			auto anotation = Label::createWithTTF("タップでスコア画面に移動", F_FONTS + JPN_FONTS, Parameter::MIDDLE * scaleFactor);
			anotation->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
			anotation->setPositionX(gameoverLabel->getPositionX());
			anotation->setPositionY(gameoverLabel->getPositionY() - anotation->getContentSize().height);
			auto sequence = Sequence::create(FadeOut::create(1.f), FadeIn::create(1.f), NULL);
			anotation->runAction(RepeatForever::create(sequence));
			this->addChild(anotation, 4);
			this->unschedule(schedule_selector(Game::main));
			mainCharacter->stopAllActions();
			UserDefault::getInstance()->setIntegerForKey(SCORE_KEY, score);
			UserDefault::getInstance()->flush();
			nextSceneFlag = true;
		}

#pragma endregion
	
	}
	

#pragma region ゲーム速度変更
	if (score != 0 && score % 10 == 0 && !speedChangeFlag)
	{
		float acceleration = score / 5000.0f;
		if (Parameter::GAME_SPEED - acceleration > 0)
		{
			speedChangeFlag = !speedChangeFlag;
			this->unschedule(schedule_selector(Game::main));
			this->schedule(schedule_selector(Game::main), Parameter::GAME_SPEED - acceleration);
		}
	}
	else if (score % 11 == 0 && speedChangeFlag)
		speedChangeFlag = !speedChangeFlag;
#pragma endregion

#pragma region 主人公初期化呼び出し
	if (mainCharacter->getPosition() == defoultPos && !hitOnlyOne
		&& mainCharacter->getNumberOfRunningActions() == 0 && !endFlag)
	{
		setCharacterDefault();
	}
#pragma endregion
}

#pragma region 主人公のジャンプアニメーション
void Game::moveCharacters(float dt)
{
	if (!endFlag)
	{
		auto mainCharacter = this->getChildByTag(1);
		auto enemy = this->getChildByTag(31);
		auto enemyPos = enemy->getPosition();

	#pragma region ジャンプ処理

		/*autoPlay機能*/
		//auto distance = mainCharacter->getPositionX() - enemy->getPositionX();
		//if (distance >= -780 && !jumpFlag && enemyPos.y == enemyDefaultPos.y)
		//	jump();

		if (jumpFlag)
		{
			auto mainCharacterPosY = mainCharacter->getPositionY();
			mainCharacterPosY += jumpPower - (9.8f * gravityPoewr);
			gravityPoewr += Parameter::ADD_GRAVITY;
			mainCharacter->setPositionY(mainCharacterPosY);
			if (mainCharacterPosY <= defoultPos.y)
			{
				jumpFlag = !jumpFlag;
				mainCharacter->setPositionY(defoultPos.y);
			}
		}

	#pragma endregion
	}
}
#pragma endregion

#pragma region 敵の再生成処理
void Game::enemyResporn(float dt)
{
	if (!endFlag)
	{
		auto enemy = this->getChildByTag(31);
		auto enemyPos = enemy->getPosition();
		auto noticeLine = this->getChildByTag(32);
		enemyPos = enemyDefaultPos;
		srand((unsigned int)time(NULL));
		int num = rand() % 3;
		auto characterImage = (Sprite*)this->getChildByTag(2);
		if (num == 2)
		{
			enemyPos.y *= 3.5f;
			characterImage->setTexture(F_IMAGE + F_MAIN_CHARACTER + FACE_NORMAL);
		}
		else
		{
			characterImage->setTexture(F_IMAGE + F_MAIN_CHARACTER + FACE_AWARENESS);
			Converter converter;
			auto seName = converter.replaceString2Char(F_SE + ALERT_SE + TYPE_MP3);
			SimpleAudioEngine::getInstance()->playEffect(seName);
		}
		enemySpeed = setEnemySpeed();
		scoreLabel->setString(SCORE_TEXT + StringUtils::toString(score));
		noticeLine->setVisible(true);
		enemy->setPosition(enemyPos);
		enemy->setVisible(true);//再生成が終了したら表示
	}
}
#pragma endregion

#pragma region ジャンプ処理
void Game::jump()
{
	auto mainCharacter = (Sprite*)this->getChildByTag(1);
	mainCharacter->stopActionByTag(101);
	mainCharacter->setTexture(F_IMAGE + F_MAIN_CHARACTER + CHARACTER_JUMP);
	jumpFlag = !jumpFlag;
	jumpPower = Parameter::DEFOULT_JUMP_POWER;
	gravityPoewr = Parameter::DEFOULT_GRAVITY_POWER;
	srand((unsigned int)time(NULL));
	int num = rand() % 2;
	Converter converter;
	auto jumpSE1 = converter.replaceString2Char(F_SE + JUMP_SE + TYPE_MP3);
	auto jumpSE2 = converter.replaceString2Char(F_SE + JUMP_SE2 + TYPE_MP3);
	switch (num)
	{
	case 0:SimpleAudioEngine::getInstance()->playEffect(jumpSE1); break;
	case 1:SimpleAudioEngine::getInstance()->playEffect(jumpSE2); break;
	default:
		break;
	}
	auto characterImage = (Sprite*)this->getChildByTag(2);
	characterImage->setTexture(F_IMAGE + F_MAIN_CHARACTER + FACE_AWARENESS);
}
#pragma endregion

#pragma region ポーズボタンの処理

void Game::pauseGame(Ref* Sender)
{
	auto mainCharacter = (Sprite*)this->getChildByTag(1);
	Sprite* life[MAX_LIFE];
	auto menuItem = (MenuItem*)this->getChildByTag(44)->getChildByTag(43);
	auto selectedButtonImage = (Sprite*)menuItem->getChildByTag(42);
	auto buttonImage = (Sprite*)menuItem->getChildByTag(41);
	if (!isPause)
	{
		this->pause();
		mainCharacter->pause();
		for (int i = 0; i < MAX_LIFE; ++i)
		{
			life[i] = (Sprite*)this->getChildByTag(20 + i);
			life[i]->pause();
		}
		SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
		buttonImage->setTexture(F_IMAGE + F_UI + RESUME_BUTTON);
		selectedButtonImage->setTexture(F_IMAGE + F_UI + RESUME_BUTTON);
	}
	else
	{
		this->resume();
		mainCharacter->resume();
		for (int i = 0; i < MAX_LIFE; ++i)
		{
			life[i] = (Sprite*)this->getChildByTag(20 + i);
			life[i]->resume();
		}
		SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
		buttonImage->setTexture(F_IMAGE + F_UI + PAUSE_BUTTON);
		selectedButtonImage->setTexture(F_IMAGE + F_UI + PAUSE_BUTTON);
	}
	isPause = !isPause;
}

#pragma endregion

#pragma region 主人公初期化設定
void Game::setCharacterDefault()
{
	auto mainCharacter = (Sprite*)this->getChildByTag(1);
	try{
		/*主人公走りモーション設定*/
		auto animation = Animation::create();
		for (int i = 0; i < Parameter::ANIMATION_MAX_NUM; ++i)
			animation->addSpriteFrameWithFile(F_IMAGE + F_ANIMATION + F_RUN + StringUtils::toString(i) + ".png");
		if (score % 10 == 0)
		{
			float acceleration = score / 7500.0f;
			if (animeSpeed - acceleration > 0.02f)
				animeSpeed -= acceleration;
		}
		animation->setDelayPerUnit(animeSpeed);
		animation->setRestoreOriginalFrame(true);
		auto runAnime = Animate::create(animation);
		auto runAnimation = RepeatForever::create(runAnime);
		runAnimation->setTag(101);
		mainCharacter->runAction(runAnimation);
		/*主人公走りモーション設定*/
	}
	catch (char* messeage)
	{
		puts(messeage);
	}
}
#pragma endregion

#pragma region チュートリアル

void Game::tutorial()
{
#pragma region 画面暗転用Spriteの作成
	auto blackBox = Rect(0,0,visibleSize.width,visibleSize.height);
	auto blackEffect = Sprite::create();
	blackEffect->setTextureRect(blackBox);
	blackEffect->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
	blackEffect->setColor(Color3B::BLACK);
	blackEffect->setOpacity(128);
	blackEffect->setTag(53);
	this->addChild(blackEffect, 4);
#pragma endregion
	
	this->unschedule(schedule_selector(Game::main));

	auto mainCharactor = this->getChildByTag(1);
	mainCharactor->pause();
	auto annotation = Label::createWithTTF("画面をタップしてきりたんぽを回避！", F_FONTS + JPN_FONTS, Parameter::LARGE * scaleFactor);
	auto sequence = Sequence::create(FadeOut::create(0.5f), FadeIn::create(0.5f), NULL);
	annotation->runAction(RepeatForever::create(sequence));
	annotation->setPosition(blackEffect->getPosition());
	annotation->setTag(54);
	this->addChild(annotation,5);

}

#pragma endregion

float setEnemySpeed()
{
	srand((unsigned int)time(NULL));
	float enemySpeed = random(1.2f, 1.9f);
	return enemySpeed;
}

void nextScene()
{
	Director::getInstance()->replaceScene(GameOver::createScene());
}