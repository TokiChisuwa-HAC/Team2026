#ifndef ENEMY_SPIKE_H_
#define ENEMY_SPIKE_H_

#include "Actor/Actor.h"
#include "Render/AnimatedMesh/AnimatedMesh.h"

class EnemySpike : public Actor {
public:
	// コンストラクタ
	EnemySpike(IWorld* world, const GSvector3& position);
	// 更新
	virtual void update(float delta_time) override;
	// 描画
	virtual void draw() const override;
	// 衝突リアクション
	virtual void react(Actor& other) override;

private:
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
};

#endif