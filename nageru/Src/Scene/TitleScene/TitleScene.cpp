#include "TitleScene.h"
#include "Assets.h"
#include "imgui/imgui.h"

// 開始
void TitleScene::start() {

}

// 更新
void TitleScene::update(float delta_time) {

}

// 描画
void TitleScene::draw() const {

}

// 終了しているか？
bool TitleScene::is_end() const {
    // 終了フラグを返す
    return is_end_;
}

// 次のシーンを返す
std::string TitleScene::next() const {
    return next_scene_name_;
}

// 終了
void TitleScene::end() {

}