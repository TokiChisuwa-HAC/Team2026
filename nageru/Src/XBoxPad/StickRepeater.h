#ifndef STICK_REPEATER_H_
#define STICK_REPEATER_H_

#include <gslib.h>

// スティックの入力をメニュー操作用のデジタル入力に変換するクラス
class StickRepeater {
public:
    // コンストラクタ
    StickRepeater(GSuint pad_no);

    // 設定
    void configure(float threshold, float start_delay, float interval);

    // 更新
    void update(float delta_time);

    // 各方向のトリガー状態を取得
    bool is_up() const;
    bool is_down() const;
    bool is_left() const;
    bool is_right() const;

    // 各方向の十字キー状態を取得
    bool is_up_button() const;
    bool is_down_button() const;
    bool is_left_button() const;
    bool is_right_button() const;

private:
    GSuint pad_no_{ 0 };

    // 設定値
    float threshold_{ 0.5f };       // デッドゾーン
    float start_delay_{ 0.4f };     // 初回リピートまでの遅延
    float repeat_interval_{ 0.1f }; // リピート間隔

    // 現在の出力状態 (このフレームでトリガーするか)
    bool up_{ false };
    bool down_{ false };
    bool left_{ false };
    bool right_{ false };
    bool up_button_{ false };
    bool down_button_{ false };
    bool left_button_{ false };
    bool right_button_{ false };

    // 押しっぱなし時間の計測用タイマー
    float timer_x_{ 0.0f };
    float timer_y_{ 0.0f };
    float timer_x_button_{ 0.0f };
    float timer_y_button_{ 0.0f };

    // 前回入力があった方向 (リセット判定用)
    // 0:なし, 1:プラス, -1:マイナス
    int last_dir_x_{ 0 };
    int last_dir_y_{ 0 };
    int last_dir_x_button_{ 0 };
    int last_dir_y_button_{ 0 };
};

#endif