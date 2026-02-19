#ifndef ANIMATED_MESH_H_
#define ANIMATED_MESH_H_

#include <gslib.h>
#include <vector>
#include <functional>
#include <vector>
#include <memory>

// アニメーションイベント構造体
// アニメーションイベント（アニメーションの指定されたタイミングで処理を実行）に関する情報を保持する。
struct AnimationEvent {
    // コンストラクタ
    AnimationEvent(GSuint motion, GSfloat time, std::function<void()> callback) :
        motion_{ motion },
        time_{ time },
        callback_{ callback } {
    }
    // イベントを発生させるモーション番号
    GSuint motion_;
    // イベントを発生させるタイミング
    GSfloat time_;
    // イベント発生時のコールバック
    std::function<void()> callback_;
};

// アニメーション付きメッシュクラス
class AnimatedMesh {
public:
    // コンストラクタ
    AnimatedMesh(GSuint id, GSuint motion = 0, bool loop = true, GSuint num_bones = 256);
    // 更新
    void update(float delta_time);
    // 描画
    void draw() const;
    // モーションの変更
    void change_motion(GSuint motion, bool loop = true);
    // 変換行列を設定する
    void transform(const GSmatrix4& matrix);
    // モーションが終了しているか？
    bool is_end_motion() const;
    // 現在のモーションの再生時間を取得
    float motion_time() const;
    // 現在のモーションの再生時間を設定
    void motion_time(float time);
    // モーションの終了時間を取得
    float motion_end_time() const;
    // ボーンのワールド変換行列を取得
    GSmatrix4 bone_matrices(int bone_no) const;
    // アニメーションイベントを登録する
    void add_event(GSuint motion, GSfloat time, std::function<void()> callback);

private:
    // アセットID
    GSuint      id_;
    // モーション番号
    GSuint	  motion_;
    // モーションタイマ
    GSfloat	  motion_timer_;
    // モーションループフラグ
    bool        motion_loop_;
    // 変換行列
    GSmatrix4   transform_;
    // 前回再生したモーション番号
    GSuint      prev_motion_;
    // 前回再生した最終アニメーションタイマ
    GSfloat     prev_motion_timer_;
    // 補間タイマ
    GSfloat     lerp_timer_;
    // ボーンのローカル用変換行列
    std::vector<GSmatrix4> local_bone_matrices_;
    // ボーンの変換行列
    std::vector<GSmatrix4> bone_matrices_;
    // 複数のアニメーションイベントを格納するためのvector
    std::vector<std::unique_ptr<AnimationEvent>> events_;
};


#endif
