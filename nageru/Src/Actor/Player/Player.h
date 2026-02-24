#ifndef PLAYER_H_
#define PLAYER_H_

#include "Actor/Actor.h"
#include "Render/AnimatedMesh/AnimatedMesh.h"

// プレイヤークラス
class Player : public Actor {
public:
	// コンストラクタ
	Player(IWorld* world, const GSvector3& position);
	// 更新
	void update(float delta_time) override;
	// 描画
	void draw() const override;
	// 衝突リアクション
	virtual void react(Actor& other) override;
	enum class State
	{
		Idle,
		Move,
		Attack,
		Damage,
		Death,
		Rise,
		Jump,
		Jumping,
		Landing,
		Hold,
		Holding,
		Throw_E,
		Start_Rotate_E,
		Rotating_E,
		Rotate_Throw_E,
	};
private:
	// フィールドとの衝突処理
	void collide_field();
	// アクターとの衝突処理
	void collide_actor(Actor& other);
	//状態の更新
	void update_state(float delta_time);
	//状態の変更
	void change_state(State state, GSuint motion, bool loop = true);
	//アイドル状態
	void idle(float delta_time);
	// 移動処理
	void move(float delta_time);
	//攻撃
	void attack(float delta_time);
	//ダメージ
	void damage(float delta_time);
	// 死亡
	void death(float delta_time);
	// 起き上がり
	void rise(float delta_time);
	// ジャンプ
	void jump(float delta_time);
	// ジャンプ中
	void jumping(float delta_time);
	// 着地
	void land(float delta_time);
	//つかみ
	void hold(float delta_time);
	//つかみ中
	void holding(float delta_time);
	//投げ
	void throw_enemy(float delta_time);
	//振り回しはじめ
	void start_throw_enemy(float delta_time);
	//振り回し中
	void rotating_enemy(float delta_time);
	//回し投げ
	void rotate_throw_enemy(float delta_time);
	//キー、ボタン入力処理
	void Input_KeyStick(float delta_time);
	// 攻撃,つかみ判定を生成する
	void generate_attack_hold_collider();

private:
	// アニメーションメッシュ
	AnimatedMesh mesh_;
	// モーション番号
	GSuint motion_;
	// モーションのループ指定
	bool motion_loop_;
	//ステート
	State           state_;
	//ステートタイマー
	float           state_timer_;
	//計算用velocity
	GSvector3 cal_v_;
	//つかみフラグ
	bool hold_frag_;
};

#endif