#ifndef TWEEN_H_
#define TWEEN_H_

#include <list>
#include "TweenUnit.h"

// Tweenモーションを管理するstaticクラス。
// 1つ1つの動きはTweenUnitクラスが行う。
// このクラスでは、複数のTweenモーションの管理を行う。
class Tween {
public:
    // TweenUnitを追加して管理下に置く
    static void add(TweenUnit* unit);
    // 管理下のTweenUnitの更新処理を呼び出す
    static void update(float delta_time);
    // 管理下のTweenUnitを全て削除する
    static void clear();
    // float型の値をTweenする
    static TweenUnit& value(float from, float to, float duration, std::function<void(float)> update_func);
    // Vector2型の値をTweenする
    static TweenUnit& vector2(const GSvector2& from, const GSvector2& to, float duration, std::function<void(const GSvector2&)> update_func);
    // Vector3型の値をTweenする
    static TweenUnit& vector3(const GSvector3& from, const GSvector3& to, float duration, std::function<void(const GSvector3&)> update_func);
    // 指定された時間後に処理を実行する
    static TweenUnit& delay_call(float delay_time, std::function<void()> callback);
    // 指定された名前のTweenをキャンセル（削除）する
    static void cancel(const std::string& name);
private:
    // インスタンス生成禁止
    Tween() = delete;
    // 複数のTweenUnitを管理するためのリスト
    static std::list<TweenUnit*> units_;
};

#endif