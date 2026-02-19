#ifndef ACTOR_H_
#define ACTOR_H_

#include <gslib.h>
#include <GStransform.h>
#include <string>
#include "Math/Collider/BoundingSphere.h"
#include "Tween/Tween.h"

class IWorld;

class Actor {
public:
	//コンストラクタ
	Actor() = default;
	//仮想デストラクタ
	virtual ~Actor() = default;
	//更新
	virtual void update(float delta_time);
	//遅延更新
	virtual void late_update(float delta_time);
	//描画
	virtual void draw() const;
	//半透明の描画
	virtual void draw_transparent() const;
	//GUIの描画
	virtual void draw_gui() const;
	//衝突リアクション
	virtual void react(Actor& other);
	//メッセージ処理
	virtual void handle_message(const std::string& message, void* param);
	//衝突判定
	void collide(Actor& other);
	//死亡する
	void die();
	//衝突しているか
	bool is_collide(const Actor& other) const;
	//死亡しているか
	bool is_dead() const;
	//名前を取得
	const std::string& name() const;
	//タグ名を取得
	const std::string& tag() const;
	//トランスフォームを取得
	const GStransform& transform() const;
	GStransform& transform();
	//移動量を取得
	GSvector3 velocity() const;
	//衝突判定データを取得
	BoundingSphere collider() const;

	// エフェクト再生
	void play_effect(GSuint id, const GSvector3& local_position, const
		GSvector3& local_rotation, const GSvector3& local_scale);
	// 指定された場所までTweenで移動する
	TweenUnit& move_to(const GSvector3& to, float duration);
	// 指定されたサイズまでTweenでリサイズする
	TweenUnit& scale_to(const GSvector3& to, float duration);

	//コピー禁止
	Actor(const Actor& other) = delete;
	Actor& operator = (const Actor& other) = delete;

	//ワールド
	IWorld* world_{ nullptr };
	//タグ名
	std::string tag_;
	//名前
	std::string name_;
	//トランスフォーム
	GStransform transform_;
	//移動量
	GSvector3 velocity_{ 0.0f, 0.0f, 0.0f };
	//衝突判定が有効か
	bool enable_collider_{ true };
	//衝突判定
	BoundingSphere collider_;
	//死亡フラグ
	bool dead_{ false };
};

#endif