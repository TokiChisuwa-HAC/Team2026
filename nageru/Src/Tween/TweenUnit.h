#ifndef TWEEN_UNIT_H_
#define TWEEN_UNIT_H_

#include <string>
#include <functional>
#include <gslib.h>
#include <GSeasing.h>

// イージング種別
enum class EaseType {
    Linear,
    EaseInBack,
    EaseInCirc,
    EaseInCubic,
    EaseInExpo,
    EaseInQuad,
    EaseInQuart,
    EaseInQuint,
    EaseInSine,
    EaseOutBack,
    EaseOutBounce,
    EaseOutCirc,
    EaseOutCubic,
    EaseOutElastic,
    EaseOutExpo,
    EaseOutQuad,
    EaseOutQuart,
    EaseOutQuint,
    EaseOutSine,
    EaseInOutBack,
    EaseInOutCirc,
    EaseInOutCubic,
    EaseInOutExpo,
    EaseInOutQuad,
    EaseInOutQuart,
    EaseInOutQuint,
    EaseInOutSine,
    Punch,
};

// Tweenの動作1つを管理するクラスの基底クラス
class TweenUnit {
public:
    // コンストラクタ
    TweenUnit(float duration);
    // 仮想デストラクタ
    virtual ~TweenUnit() = default;
    // 更新処理
    void update(float delta_time);
    // 終了したかどうか
    bool is_finished() const;
    // イージング種別を設定する
    TweenUnit& ease(EaseType ease_type);
    // Back, Elastic, Punchでビヨンビヨンとなる勢いを設定する。値が大きいほど、ビヨンビヨンする。
    TweenUnit& overshoot(float overshoot);
    // 開始遅延時間を設定する
    TweenUnit& delay(float delay);
    // 完了時のコールバックを設定する
    TweenUnit& on_complete(std::function<void()> complete_func);
    // 名前を取得する
    const std::string& name() const;
    // 名前を設定する
    TweenUnit& name(const std::string& name);
    // コピー禁止
    TweenUnit(const TweenUnit& other) = delete;
    TweenUnit& operator = (const TweenUnit& other) = delete;

protected:
    // 派生クラスごとの更新処理
    virtual void on_update(float progress) {}

private:
    float duration_{ 0.0f };                  // アニメーション時間（フレーム）
    float elapsed_{ 0.0f };                   // 経過時間（フレーム）
    bool is_finished_{ false };               // 終了したか
    EaseType ease_type_{ EaseType::Linear };  // イージング種別
    float overshoot_{ 1.70158f };             // Elastic、Back、Punchで使う勢い
    float delay_{ 0.0f };                     // 開始遅延時間
    std::function<void()> complete_func_{};   // 完了時のコールバック
    std::string name_{};                      // 名前（キャンセル処理に使う）

    // 値にイージングを適用する
    static float apply_ease(float value, EaseType ease_type, float overshoot);
};

#endif