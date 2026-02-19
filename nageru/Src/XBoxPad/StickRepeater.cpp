#include "StickRepeater.h"
#include <cmath>

// コンストラクタ
StickRepeater::StickRepeater(GSuint pad_no)
    : pad_no_(pad_no)
{
}

// 設定
void StickRepeater::configure(float threshold, float start_delay, float interval) {
    threshold_ = threshold;
    start_delay_ = start_delay;
    repeat_interval_ = interval;
}

// 更新
void StickRepeater::update(float delta_time) {
    // スティックの状態を取得
    GSvector2 stick = { 0.0f, 0.0f };
    gsXBoxPadGetLeftAxis(pad_no_, &stick); // 左スティックを取得

    int cur_dir_x = 0;
    if (stick.x > threshold_) cur_dir_x = 1;        // 右
    else if (stick.x < -threshold_) cur_dir_x = -1; // 左

    right_ = false;
    left_ = false;

    if (cur_dir_x == 0) {
        // 入力なしに戻ったらリセット
        timer_x_ = 0.0f;
        last_dir_x_ = 0;
    }
    else {
        // 方向が変わった、または初回入力された場合
        if (cur_dir_x != last_dir_x_) {
            timer_x_ = 0.0f; // タイマーリセット

            // 即座に反応させる
            if (cur_dir_x == 1) right_ = true;
            else left_ = true;
        }
        else {
            // 同じ方向に押しっぱなしにしている場合
            timer_x_ += delta_time;

            // 開始遅延を超えているかチェック
            if (timer_x_ >= start_delay_) {
                // タイマーを間隔分だけ戻してループさせる
                timer_x_ -= repeat_interval_;

                if (cur_dir_x == 1) right_ = true;
                else left_ = true;
            }
        }
        last_dir_x_ = cur_dir_x; // 今回の方向を記録
    }

    int cur_dir_y = 0;
    if (stick.y > threshold_) cur_dir_y = 1;        // 上
    else if (stick.y < -threshold_) cur_dir_y = -1; // 下

    up_ = false;
    down_ = false;

    if (cur_dir_y == 0) {
        timer_y_ = 0.0f;
        last_dir_y_ = 0;
    }
    else {
        if (cur_dir_y != last_dir_y_) {
            timer_y_ = 0.0f;
            if (cur_dir_y == 1) up_ = true;
            else down_ = true;
        }
        else {
            timer_y_ += delta_time;
            if (timer_y_ >= start_delay_) {
                timer_y_ -= repeat_interval_;
                if (cur_dir_y == 1) up_ = true;
                else down_ = true;
            }
        }
        last_dir_y_ = cur_dir_y;
    }


    int dir_x = 0;
    if (gsXBoxPadButtonState(0, GS_XBOX_PAD_RIGHT)) dir_x = 1;      // 右
    else if (gsXBoxPadButtonState(0, GS_XBOX_PAD_LEFT)) dir_x = -1; // 左

    right_button_ = false;
    left_button_ = false;

    if (dir_x == 0) {
        // 入力なしに戻ったらリセット
        timer_x_button_ = 0.0f;
        last_dir_x_button_ = 0;
    }
    else {
        // 方向が変わった、または初回入力された場合
        if (dir_x != last_dir_x_button_) {
            timer_x_button_ = 0.0f; // タイマーリセット

            // 即座に反応させる
            if (dir_x == 1) right_button_ = true;
            else left_button_ = true;
        }
        else {
            // 同じ方向に押しっぱなしにしている場合
            timer_x_button_ += delta_time;

            // 開始遅延を超えているかチェック
            if (timer_x_button_ >= start_delay_) {
                // タイマーを間隔分だけ戻してループさせる
                timer_x_button_ -= repeat_interval_;

                if (dir_x == 1) right_button_ = true;
                else left_button_ = true;
            }
        }
        last_dir_x_button_ = dir_x; // 今回の方向を記録
    }

    int dir_y = 0;
    if (gsXBoxPadButtonState(0, GS_XBOX_PAD_UP)) dir_y = 1;         // 上
    else if (gsXBoxPadButtonState(0, GS_XBOX_PAD_DOWN)) dir_y = -1; // 下

    up_button_ = false;
    down_button_ = false;

    if (dir_y == 0) {
        timer_y_button_ = 0.0f;
        last_dir_y_button_ = 0;
    }
    else {
        if (dir_y != last_dir_y_button_) {
            timer_y_button_ = 0.0f;
            if (dir_y == 1) up_button_ = true;
            else down_button_ = true;
        }
        else {
            timer_y_button_ += delta_time;
            if (timer_y_button_ >= start_delay_) {
                timer_y_button_ -= repeat_interval_;
                if (dir_y == 1) up_button_ = true;
                else down_button_ = true;
            }
        }
        last_dir_y_button_ = dir_y;
    }
}

bool StickRepeater::is_up() const { return up_; }
bool StickRepeater::is_down() const { return down_; }
bool StickRepeater::is_left() const { return left_; }
bool StickRepeater::is_right() const { return right_; }

bool StickRepeater::is_up_button() const { return up_button_; }
bool StickRepeater::is_down_button() const { return down_button_; }
bool StickRepeater::is_left_button() const { return left_button_; }
bool StickRepeater::is_right_button() const { return right_button_; }