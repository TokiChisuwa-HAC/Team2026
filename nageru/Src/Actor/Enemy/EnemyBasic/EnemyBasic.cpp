#include "EnemyBasic.h"
#include "World/IWorld.h"
#include "Field/Field.h"
#include "Math/Line.h"
#include "Assets.h"
#include <GSeffect.h>

// モーション番号
enum {
	MotionIdle = 3,			// アイドル状態
	MotionDizzy = 2,		// 気絶
	MotionWalk = 10,		// 通常移動
	MotionAttack = 0,		// 攻撃
	MotionTurnLeft = 16,	// 左に振り向く
	MotionTurnRight = 17,	// 右に振り向く
	MotionJump = 1,			// ジャンプ
	MotionTakeDamage = 15,	// ダメージを受ける
};

// 自分の高さ
const float EnemyHeight{ 1.0f };
// 衝突判定用の半径
const float EnemyRadius{ 0.6f };
// 足元のオフセット
const float FootOffset{ 0.1f };

EnemyBasic::EnemyBasic(IWorld* world, const GSvector3& position) :
	mesh_{ Mesh_EnemyBasic, MotionIdle, true },
	motion_{ MotionIdle },
	motion_loop_{ true } {
	world_ = world;
	tag_ = "EnemyTag";
	name_ = "EnemyBasic";
	// 衝突判定球の設定
	collider_ = BoundingSphere{ EnemyRadius, GSvector3{ 0.0f, EnemyHeight, 0.0f } };
	// 座標の初期化
	transform_.position(position);
	// メッシュの変換行列を初期化
	mesh_.transform(transform_.localToWorldMatrix());

	//タイマー初期化
	state_timer_ = 0.0f;
}

// 更新
void EnemyBasic::update(float delta_time) {
	//状態の更新
	update_state(delta_time);
	// 重力で下向きに加速
	velocity_.y += world_->field()->gravity() * delta_time;
	// y方向に移動
	transform_.translate(0.0f, velocity_.y, 0.0f);
	// フィールドとの衝突判定
	collide_field();
	// モーションを変更
	mesh_.change_motion(motion_, motion_loop_);
	// メッシュを更新
	mesh_.update(delta_time);
	// ワールド行列を設定
	mesh_.transform(transform_.localToWorldMatrix());
	// エフェクトに自身のワールド変換行列を設定
	GSmatrix4 world = transform_.localToWorldMatrix();
	// ワールド変換行列を設定
	gsSetEffectMatrix(effect_handle_, &world);
}

//状態の更新
void EnemyBasic::update_state(float delta_time) {
	switch (state_) {
	case State::Idle:idle(delta_time); break;
	case State::Patrol:patrol(delta_time); break;
	case State::Chase:chase(delta_time); break;
	case State::Attack:attack(delta_time); break;
	case State::Down:down(delta_time); break;
	case State::Hold:hold(delta_time); break;
	case State::Thrown:thrown(delta_time); break;
	case State::Damage:damage(delta_time); break;
	case State::Die:enemy_die(delta_time); break;
	}
	state_timer_ += delta_time;
}

//状態の変更
void EnemyBasic::change_state(State state, GSuint motion, bool loop) {
	if (state_ == state && motion_ == motion) {
		return;
	}

	motion_ = motion;
	motion_loop_ = loop;
	state_ = state;
	state_timer_ = 0.0f;
}

//アイドル状態
void EnemyBasic::idle(float delta_timer) {

}

//パトロール状態
void EnemyBasic::patrol(float delta_timer) {

}

//追跡状態
void EnemyBasic::chase(float delta_timer) {

}

//攻撃状態
void EnemyBasic::attack(float delta_timer) {

}

//ダウン状態
void EnemyBasic::down(float delta_timer) {

}

//捕まれ状態
void EnemyBasic::hold(float delta_timer) {

}

//投げられ状態
void EnemyBasic::thrown(float delta_timer) {

}

//ダメージ状態
void EnemyBasic::damage(float delta_timer) {

}

//死状態
void EnemyBasic::enemy_die(float delta_timer) {

}

// フィールドとの衝突判定
void EnemyBasic::collide_field() {
	// 壁との衝突判定（球体との判定）
	GSvector3 center; // 衝突後の球体の中心座標
	if (world_->field()->collide(collider(), &center)) {
		// y座標は変更しない
		center.y = transform_.position().y;
		// 補正後の座標に変更する
		transform_.position(center);
	}
	// 地面との衝突判定（線分との交差判定）
	GSvector3 position = transform_.position();
	Line line;
	line.start = position + collider_.center;
	line.end = position + GSvector3{ 0.0f, -FootOffset, 0.0f };
	GSvector3 intersect;            // 地面との交点
	// 衝突したフィールド用アクター
	Actor* field_actor{ nullptr };
	// 親をリセットしておく
	transform_.parent(nullptr);
	if (world_->field()->collide(line, &intersect, nullptr, &field_actor)) {
		// 交差した点からy座標のみ補正する
		position.y = intersect.y;
		// 座標を変更する
		transform_.position(position);
		// 重力を初期化する
		velocity_.y = 0.0f;
		// フィールド用のアクタークラスと衝突したか？
		if (field_actor != nullptr) {
			// 衝突したフィール用のアクターを親のトランスフォームクラスとして設定
			transform_.parent(&field_actor->transform());
		}
	}
}

// アクターとの衝突処理
void EnemyBasic::collide_actor(Actor& other) {
	// y座標を除く座標を求める
	GSvector3 position = transform_.position();
	position.y = 0.0f;
	GSvector3 target = other.transform().position();
	target.y = 0.0f;
	// 相手との距離
	float distance = GSvector3::distance(position, target);
	// 衝突判定球の半径同士を加えた長さを求める
	float length = collider_.radius + other.collider().radius;
	// 衝突判定球の重なっている長さを求める
	float overlap = length - distance;
	// 重なっている部分の半分の距離だけ離れる移動量を求める
	GSvector3 v = (position - target).getNormalized() * overlap * 0.5f;
	transform_.translate(v, GStransform::Space::World);
	// フィールドとの衝突判定
	collide_field();
}

// 描画
void EnemyBasic::draw() const {
	// メッシュの描画
	mesh_.draw();
	// 当たり判定をテスト描画
	collider().draw();
}

// 衝突リアクション
void EnemyBasic::react(Actor& other) {

}