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

private:
	// フィールドとの衝突処理
	void collide_field();
	// アクターとの衝突処理
	void collide_actor(Actor& other);
	// 移動処理
	void move(float delta_time);

private:
	// アニメーションメッシュ
	AnimatedMesh mesh_;
	// モーション番号
	GSuint motion_;
	// モーションのループ指定
	bool motion_loop_;
};

#endif