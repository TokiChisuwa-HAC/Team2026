#include "World.h"
#include "Field/Field.h"
#include "Actor/Actor.h"
#include "Assets.h"
#include <GSeffect.h>

// デストラクタ
World::~World() {
    clear();
}

// 更新
void World::update(float delta_time) {
    // フィールドの更新
    field_->update(delta_time);
    // アクターの更新
    actors_.update(delta_time);
    // アクターの衝突
    actors_.collide();
    // アクターの遅延更新
    actors_.late_update(delta_time);
    // アクターの消去
    actors_.remove();
    // カメラの更新
    camera_->update(delta_time);
    // ライトの更新
    light_->update(delta_time);
    // エフェクトの更新処理
    gsUpdateEffect(delta_time);
}

// 描画
void World::draw() const {
    // カメラの設定
    camera_->draw();
    // エフェクト用のカメラを設定
    gsSetEffectCamera();
    // ライトの設定
    light_->draw();
    // シャドウマップの描画
    gsDrawShadowMap(World::shadow_map_callback, (void*)this);
    // フィールドの描画
    field_->draw();
    // アクターの描画
    actors_.draw();
    // 半透明アクターの描画
    actors_.draw_transparent();
    // エフェクトの描画
    gsDrawEffect();
    // GUIの描画
    actors_.draw_gui();
}

// 消去
void World::clear() {
    // アクターを消去
    actors_.clear();
    // カメラを消去
    delete camera_;
    camera_ = nullptr;
    // ライトを消去
    delete light_;
    light_ = nullptr;
    // フィールドを消去
    delete field_;
    field_ = nullptr;
}

// カメラの追加
void World::add_camera(Actor* camera) {
    delete camera_;	// 現在のカメラを削除
    camera_ = camera;
}

// ライトの追加
void World::add_light(Actor* light) {
    delete light_;	// 現在のライトを削除
    light_ = light;
}

// フィールドの追加
void World::add_field(Field* field) {
    delete field_;	// 現在のフィールドを削除
    field_ = field;
}

// アクターの追加
void World::add_actor(Actor* actor) {
    actors_.add(actor);
}

// アクターの検索
Actor* World::find_actor(const std::string& name) const {
    return actors_.find(name);
}

// 指定したタグ名を持つアクターの検索
std::vector<Actor*> World::find_actor_with_tag(const std::string& tag) const {
    return actors_.find_with_tag(tag);
}

// アクター数を返す
int World::count_actor() const {
    return actors_.count();
}

// 指定したタグ名を持つアクター数を返す
int World::count_actor_with_tag(const std::string& tag) const {
    return actors_.count_with_tag(tag);
}

// メッセージ送信
void World::send_message(const std::string& message, void* param) {
    actors_.send_message(message, param);
}

// フリーカメラの座標取得
GSvector3 World::free_camera_eye() const {
    return free_camera_eye_;
}

// フリーカメラの注視点取得
GSvector3 World::free_camera_at() const {
    return free_camera_at_;
}

// フリーカメラの座標設定
void World::free_camera_eye(GSvector3 eye) {
    free_camera_eye_ = eye;
}

// フリーカメラの注視点設定
void World::free_camera_at(GSvector3 at) {
    free_camera_at_ = at;
}

// カメラの取得
Actor* World::camera() {
    return camera_;
}

// ライトの取得
Actor* World::light() {
    return light_;
}

// フィールドの取得
Field* World::field() {
    return field_;
}

// シャドウマップの描画用の関数
void World::shadow_map_callback(void* param, const GSmatrix4*, const GSmatrix4*) {
    World* self = (World*)param;
    // シャドウマップにはアクターのみ描画
    self->actors_.draw();
    // シャドウマップにフィールド用のアクターを描画
    self->field_->draw_actors();
}