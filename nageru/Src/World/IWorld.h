#ifndef IWORLD_H_
#define IWORLD_H_

#include <string>
#include <vector>
#include <gslib.h>

class Actor;
class Field;

class IWorld {
public:
    // 仮想デストラクタ
    virtual ~IWorld() = default;

    // アクターを追加
    virtual void add_actor(Actor* actor) = 0;
    // アクターの検索
    virtual Actor* find_actor(const std::string& name) const = 0;
    // 指定したタグ名を持つアクターの検索
    virtual std::vector<Actor*> find_actor_with_tag(const std::string& tag) const = 0;
    // アクター数を返す
    virtual int count_actor() const = 0;
    // 指定したタグ名を持つアクター数を返す
    virtual int count_actor_with_tag(const std::string& tag) const = 0;
    // メッセージの送信
    virtual void send_message(const std::string& message, void* param = nullptr) = 0;

    // フリーカメラの座標取得
    virtual GSvector3 free_camera_eye() const = 0;
    // フリーカメラの注視点取得
    virtual GSvector3 free_camera_at() const = 0;
    // フリーカメラの座標設定
    virtual void free_camera_eye(GSvector3 eye) = 0;
    // フリーカメラの注視点設定
    virtual void free_camera_at(GSvector3 at) = 0;

    // フィールドの取得
    virtual Field* field() = 0;
    // カメラの取得
    virtual Actor* camera() = 0;
    // ライトの取得
    virtual Actor* light() = 0;
};

#endif