#ifndef YOSHINOBU_SCENE_H_
#define YOSHINOBU_SCENE_H_

#include "Scene/IScene.h"
#include "World/World.h"

// ゲームプレイシーン
class YoshinobuScene : public IScene {
public:
    // 開始
    virtual void start() override;
    // 更新
    virtual void update(float delta_time) override;
    // 描画
    virtual void draw() const override;
    // 終了しているか？
    virtual bool is_end() const override;
    // 次のシーンを返す
    virtual std::string next() const override;
    // 終了
    virtual void end() override;
private:
    // ワールドクラス
    World world_;
    // 終了フラグ
    bool is_end_{ false };
    // 次のシーン名
    std::string next_scene_name_;
};

#endif