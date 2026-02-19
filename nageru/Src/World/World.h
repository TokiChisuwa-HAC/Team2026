#ifndef WORLD_H_
#define WORLD_H_

#include "IWorld.h"
#include "Actor/ActorManager.h"
#include <gslib.h>

class World : public IWorld {
public:
    // コンストラクタ
    World() = default;
    // デストラクタ
    ~World();
    // 更新
    void update(float delta_time);
    // 描画
    void draw() const;
    // 消去
    void clear();
    // カメラの追加
    void add_camera(Actor* camera);
    // ライトの追加
    void add_light(Actor* light);
    // フィールドの追加
    void add_field(Field* field);

    // アクターを追加
    virtual void add_actor(Actor* actor) override;
    // アクターの検索
    virtual Actor* find_actor(const std::string& name) const override;
    // 指定したタグ名を持つアクターの検索
    virtual std::vector<Actor*> find_actor_with_tag(const std::string& tag) const override;
    // アクター数を返す
    virtual int count_actor() const override;
    // 指定したタグ名を持つアクター数を返す
    virtual int count_actor_with_tag(const std::string& tag) const override;
    // メッセージの送信
    virtual void send_message(const std::string& message, void* param = nullptr) override;

    // フリーカメラの座標取得
    virtual GSvector3 free_camera_eye() const override;
    // フリーカメラの注視点取得
    virtual GSvector3 free_camera_at() const override;
    // フリーカメラの座標設定
    virtual void free_camera_eye(GSvector3 eye) override;
    // フリーカメラの注視点設定
    virtual void free_camera_at(GSvector3 at) override;

    // カメラの取得
    virtual Actor* camera() override;
    // ライトの取得
    virtual Actor* light() override;
    // フィールドの取得
    virtual Field* field() override;

    // シャドウマップの描画用の関数
    static void shadow_map_callback(void* param, const GSmatrix4*, const GSmatrix4*);

    // コピー禁止
    World(const World& other) = delete;
    World& operator = (const World& other) = delete;

private:
    // アクターマネージャー
    ActorManager  actors_;
    // ライト
    Actor* light_{ nullptr };
    // カメラ
    Actor* camera_{ nullptr };
    // フィールド
    Field* field_{ nullptr };

    // フリーカメラの座標
    GSvector3 free_camera_eye_{ 0.0f, 0.0f, 0.0f };
    // フリーカメラの注視点
    GSvector3 free_camera_at_{ 0.0f, 0.0f, 1.0f };
};

#endif