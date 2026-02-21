#ifndef ENEMY_BASIC_H_
#define ENEMY_BASIC_H_

#include "Actor/Actor.h"
#include "Render/AnimatedMesh/AnimatedMesh.h"

// 敵クラス
class EnemyBasic : public Actor {
public:
	//敵の状態
	enum class State {
		Idle,			//アイドル状態
		Patrol,			//パトロール
		Chase,			//追跡
		Attack,			//攻撃
		Down,			//ダウン
		Hold,			//つかまれ状態
		Thrown,			//投げられ状態
		Damage,			//ダメージ状態(仮)
		Die,			//死(仮)
	};


public:
	// コンストラクタ
	EnemyBasic(IWorld* world, const GSvector3& position);
	// 更新
	virtual void update(float delta_time) override;
	// 描画
	virtual void draw() const override;
	// 衝突リアクション
	virtual void react(Actor& other) override;

private:
	//状態の更新
	void update_state(float delta_time);
	//状態の変更
	void change_state(State state, GSuint motion, bool loop = true);
	//アイドル状態
	void idle(float delta_time);
	//パトロール状態
	void patrol(float delta_time);
	//追跡状態
	void chase(float delta_time);
	//攻撃状態
	void attack(float delta_time);
	//ダウン状態
	void down(float delta_time);
	//捕まれ状態
	void hold(float delta_time);
	//投げられ状態
	void thrown(float delta_time);
	//ダメージ状態
	void damage(float delta_time);
	//死
	void enemy_die(float delta_time);

	// フィールドとの衝突処理
	void collide_field();
	// アクターとの衝突処理
	void collide_actor(Actor& other);

private:
	// アニメーションメッシュ
	AnimatedMesh mesh_;
	// モーション番号
	GSuint motion_;
	// モーションのループ指定
	bool motion_loop_;
	// エフェクトハンドル
	GSuint effect_handle_;
	//状態の変数
	State state_;

	//タイマー変数定義
	float state_timer_;
};

#endif