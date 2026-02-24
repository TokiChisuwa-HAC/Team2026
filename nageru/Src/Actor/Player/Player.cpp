#include "Player.h"
#include "World/IWorld.h"
#include "Field/Field.h"
#include "Math/Line.h"
#include "Assets.h"
#include "Actor/AttackCollider/AttackCollider.h"

// モーション番号
enum {
    MotionIdle = 14,        // アイドル状態
    MotionTakeDamage = 11,  // ダメージを受ける
    MotionDizzy = 10,       // 気絶
    MotionDeath = 9,        // 死亡
    MotionAttack = 0,       // 攻撃
    MotionUpperAttack = 1,  // アッパー攻撃
    MotionGrabStart = 12,   // つかみ開始
    MotionGrabMid = 13,     // つかみ中
    MotionSwingMid = 32,    // 振り回し開始
    MotionSwingStart = 33,  // 振り回し中
    MotionThrow = 34,       // 放り投げる
    MootionJumpStart = 24,  // ジャンプ開始
    MotionJumpMid = 22,     // ジャンプ中
    MotionJumpEnd = 23,     // 着地
    MotionWalkBack = 37,    // 歩き（後ろ方向）
    MotionWalkForward = 38, // 歩き（前方向）
    MotionWalkLeft = 39,    // 歩き（左方向）
    MotionWalkRight = 40,   // 歩き（右方向）
    MotionRunBack = 26,     // 走り（後ろ方向）
    MotionRunForward = 27,  // 走り（前方向）
    MotionRunLeft = 28,     // 走り（左方向）
    MotionRunRight = 29,    // 走り（右方向）
    MotionDush = 30,        // ダッシュ

    MotionEmote1 = 3,       // エモート1
    MotionEmote2 = 4,       // エモート2
    MotionEmote3 = 35,      // エモート3
    MotionEmote4 = 36,      // エモート4
};

// 移動速度
const float WalkSpeed{ 0.025f };
//ジャンプ中速度
const float JumpingSpeed{ 0.008f };
// 自分の高さ
const float PlayerHeight{ 0.7f };
// 衝突判定用の半径
const float PlayerRadius{ 0.45f };
// 足元のオフセット
const float FootOffset{ 0.1f };

// コンストラクタ
Player::Player(IWorld* world, const GSvector3& position) :
	mesh_{ Mesh_Player, MotionIdle, true },
	motion_{ MotionIdle },
	motion_loop_{ true },
    state_{State::Idle} {
	world_ = world;
	tag_ = "PlayerTag";
	name_ = "Player";
	// 衝突判定球の設定
	collider_ = BoundingSphere{ PlayerRadius, GSvector3{ 0.0f, PlayerHeight, 0.0f } };
	// 座標の初期化
	transform_.position(position);
	// メッシュの変換行列を初期化
	mesh_.transform(transform_.localToWorldMatrix());
}

// 更新
void Player::update(float delta_time) {
    // 状態の更新
    update_state(delta_time);
	// 入力処理
	move(delta_time);
	// 重力で下向きに加速
	velocity_.y += world_->field()->gravity() * delta_time;
	// y方向に移動
	transform_.translate(0.0f, velocity_.y, 0.0f);
	// フィールドとの衝突判定
	collide_field();
	// モーションを変更
	mesh_.change_motion(motion_, motion_loop_);
	// メッシュのモーションを更新
	mesh_.update(delta_time);
	// ワールド変換行列を設定
    mesh_.transform(transform_.localToWorldMatrix());
    //キー、ボタン入力処理
    Input_KeyStick(delta_time);
}
//状態の更新
void Player::update_state(float delta_time) {
    switch (state_) {
    case State::Idle:  idle(delta_time); break;
    case State::Move:  move(delta_time); break;
    case State::Attack: attack(delta_time); break;
    case State::Damage: damage(delta_time); break;
    case State::Death: death(delta_time); break;
    case State::Rise: rise(delta_time); break;
    case State::Jump: jump(delta_time); break;
    case State::Jumping: jumping(delta_time); break;
    case State::Landing:land(delta_time); break;
    case State::Hold:hold(delta_time); break;
    case State::Holding:holding(delta_time); break;
    case State::Throw_E:throw_enemy(delta_time); break;
    case State::Start_Rotate_E:start_throw_enemy(delta_time); break;
    case State::Rotating_E:rotating_enemy(delta_time); break;
    case State::Rotate_Throw_E:rotate_throw_enemy(delta_time); break;
    }
    state_timer_ += delta_time;
}
//状態の変更
void Player::change_state(State state, GSuint motion, bool loop){
    motion_ = motion;
    motion_loop_ = loop;
    state_ = state;
    state_timer_ = 0.0f;
}
//キー、ボタン入力処理
void Player::Input_KeyStick(float delta_time) {
    // カメラの前方向ベクトルを取得
    GSvector3 forward = world_->camera()->transform().forward();
    forward.y = 0.0f;
    // カメラの右方向ベクトルを取得
    GSvector3 right = world_->camera()->transform().right();
    GSvector2 leftStick;
    gsXBoxPadGetLeftAxis(0, &leftStick);
    switch (state_) {
        //Idle,Move時
    case State::Idle:
    case State::Move:
        cal_v_ = { 0.0,0.0,0.0 };
        if (leftStick.y > 0.5f || gsGetKeyState(GKEY_W)) {
            cal_v_ += forward;
        }
        if (leftStick.y < -0.5f || gsGetKeyState(GKEY_S)) {
            cal_v_ -= forward;
        }
        if (leftStick.x > 0.5f || gsGetKeyState(GKEY_D)) {
            cal_v_ += right;
        }
        if (leftStick.x < -0.5f || gsGetKeyState(GKEY_A)) {
            cal_v_ -= right;
        }

        cal_v_ = cal_v_.normalized() * WalkSpeed * delta_time;

        // 移動量のxz成分だけ更新
        velocity_.x = cal_v_.x;
        velocity_.z = cal_v_.z;

        //攻撃判定
        if (gsXBoxPadButtonTrigger(0, GS_XBOX_PAD_RIGHT_SHOULDER) || gsGetMouseButtonTrigger(GMOUSE_BUTTON_1)) {
            // 自分の前方に衝突判定を出現させる
            generate_attack_hold_collider();
            change_state(State::Attack, MotionAttack, false);
            return;
        }
        //つかみ判定
        if (gsXBoxPadButtonTrigger(0, GS_XBOX_PAD_B) || gsGetMouseButtonTrigger(GMOUSE_BUTTON_2)) {
            //判定
            hold_frag_ = true;
            // 自分の前方に衝突判定を出現させる
            generate_attack_hold_collider();
            change_state(State::Hold, MotionGrabStart,false);
            return;
        }
        //ジャンプ判定
        if (gsXBoxPadButtonTrigger(0, GS_XBOX_PAD_A) || gsGetKeyTrigger(GKEY_SPACE)) {
            // ジャンプ開始状態へ
            change_state(State::Jump, MootionJumpStart, false);
            // ジャンプ
            velocity_.y = 0.08f;
            return;

        }
        break;
    case State::Jumping:
        cal_v_ *= 0.8;
        if (leftStick.y > 0.5f || gsGetKeyState(GKEY_W)) {
            cal_v_ += forward;
        }
        if (leftStick.y < -0.5f || gsGetKeyState(GKEY_S)) {
            cal_v_ -= forward;
        }
        if (leftStick.x > 0.5f || gsGetKeyState(GKEY_D)) {
            cal_v_ += right;
        }
        if (leftStick.x < -0.5f || gsGetKeyState(GKEY_A)) {
            cal_v_ -= right;
        }

        cal_v_ = cal_v_.normalized() * JumpingSpeed * delta_time;

        // 移動量のxz成分だけ更新
        velocity_.x = cal_v_.x;
        velocity_.z = cal_v_.z;

        break;

    case State::Rotating_E:
        break;

    }
}

//アイドル状態
void Player::idle(float delta_time) {
    // 何もしなければアイドル状態
    change_state(State::Idle, MotionIdle);
}
// 移動処理
void Player::move(float delta_time) {
    // 平行移動する（ワールド基準）
    transform_.translate(velocity_, GStransform::Space::World);
}
//攻撃
void Player::attack(float delta_time) {
    // 移動しない
    velocity_ = GSvector3{ 0, 0, 0 };
    // 攻撃のモーションが終了したら移動中に遷移
    if (state_timer_ >= mesh_.motion_end_time()) {
        // 攻撃モーションが終了したら移動中に遷移
        change_state(State::Idle, MotionIdle);
    }
}
//ダメージ
void Player::damage(float delta_time) {
}
// 死亡
void Player::death(float delta_time) {
}
// 起き上がり
void Player::rise(float delta_time) {
}
// ジャンプ
void Player::jump(float delta_time) {
    if (state_timer_ >= 4) {
        // ある程度したら、すぐにジャンプ中モーションへ
        change_state(State::Jumping, MotionJumpMid, false);
    }
}
// ジャンプ中
void Player::jumping(float delta_time) {
    // 平行移動する（ワールド基準）
    transform_.translate(velocity_, GStransform::Space::World);
}
// 着地
void Player::land(float delta_time) {
    if (state_timer_ >= 7) {
        // ある程度したら、すぐに通常状態へ
        change_state(State::Idle, MotionIdle);
    }
}
// つかみ
void Player::hold(float delta_time) {
    if (state_timer_ >= mesh_.motion_end_time()) {
        // 攻撃モーションが終了したら移動中に遷移
        change_state(State::Idle, MotionIdle);
        hold_frag_ = false;
    }
}
// つかみ中
void Player::holding(float delta_time) {
}
// 投げ
void Player::throw_enemy(float delta_time) {
}
// 振り回しはじめ
void Player::start_throw_enemy(float delta_time) {
}
// 振り回し中
void Player::rotating_enemy(float delta_time) {
}
// 回し投げ
void Player::rotate_throw_enemy(float delta_time) {
}

// フィールドとの衝突判定
void Player::collide_field() {
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
        if (state_ == State::Jumping) {
            // 速度を止める
            velocity_ = GSvector3::zero();
            // 着地状態へ
            change_state(State::Landing, MotionJumpEnd, false);
        }

    }
}

// アクターとの衝突判定
void Player::collide_actor(Actor& other) {
    // y座標をのぞく座標を求める
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
void Player::draw() const {
    // メッシュを描画
    mesh_.draw();
    // 当たり判定をテスト描画
    collider().draw();
}

// 衝突リアクション
void Player::react(Actor& other) {

}

// 攻撃判定を生成する
void Player::generate_attack_hold_collider() {
    // 攻撃判定を出現させる場所の距離
    const float AttackColliderDistance{ 1.5f };
    // 攻撃判定の半径
    const float AttackColliderRadius{ 1.0f };
    // 攻撃判定を出す場所の高さ
    const float AttackColliderHeight{ 1.0f };

    // 攻撃判定が有効になるまでの遅延時間
    const float AttackCollideDelay{ 0.0f };
    // 攻撃判定の寿命
    const float AttackCollideLifeSpan{ 30.0f };

    // 衝突判定を出現させる座標を求める（前方の位置）
    GSvector3 position = transform_.position() + transform_.forward() * AttackColliderDistance;
    // 高さの補正（足元からの高さ）
    position.y += AttackColliderHeight;
    // 衝突判定用の球を作成
    BoundingSphere collider{ AttackColliderRadius, position };
    // 衝突判定を出現させる
    if (hold_frag_){
        //つかみ判定
        world_->add_actor(new AttackCollider{ world_, collider,
        "PlayerTag", "AttackCollider", "PlayerHoldTag", AttackCollideLifeSpan, AttackCollideDelay });
    }
    else {
        //攻撃判定
        world_->add_actor(new AttackCollider{ world_, collider,
        "PlayerTag", "AttackCollider", "PlayerAttackTag", AttackCollideLifeSpan, AttackCollideDelay });
    }
}
