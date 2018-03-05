/// <summary> ゲームシーン用クラスの定義ファイル
/// @author Sigure
/// @date 2017/04/12
/// </summary>
#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__
#include "cocos2d.h"

class Game : public cocos2d::Layer
{
public:

	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(Game);
	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	void main(float dt);
	void moveCharacters(float dt);
	void enemyResporn(float dt);
	void setCharacterDefault();
	void pauseGame(cocos2d::Ref* Sender);
	void jump();
	void tutorial();

	int score; //敵が画面端に到達した回数
	int hitCounter; //敵と接触した回数
	int jumpPower;//ジャンプ時の重力抵抗
	float animeSpeed;//主人公アニメーションの速さ
	float enemySpeed;//敵速度の倍率
	float gravityPoewr;//ジャンプ時の重力
	float scaleFactor;//スケールの倍率
	bool endFlag; //ゲーム終了フラグ
	bool hitOnlyOne; //一度の接触で一度しか接触しないためのフラグ
	bool jumpFlag;//ジャンプ状態のフラグ
	bool speedChangeFlag;//ゲーム速度変更時のフラグ
	bool isPause;//ポーズ状態のフラグ
	bool nextSceneFlag;//画面遷移のフラグ
	bool isFirstPlay;//初プレイ時のフラグ
	cocos2d::Size visibleSize;//画面サイズ
	cocos2d::Vec2 origin;
	cocos2d::Vec2 defoultPos; //主人公スプライトの初期位置
	cocos2d::Vec2 enemyDefaultPos; //敵スプライトの初期位置
	cocos2d::Vec2 outOfWindowBGPos; //画面外に生成される背景スプライトの初期位置
	cocos2d::Label* scoreLabel;//スコア表示用ラベル
	static const int MAX_LIFE;
};

#endif