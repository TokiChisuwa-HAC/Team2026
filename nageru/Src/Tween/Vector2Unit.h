#ifndef VECTOR2_UNIT_H_
#define VECTOR2_UNIT_H_

#include "TweenUnit.h"

// Vector2型を動かすためのTweenUnit
class Vector2Unit : public TweenUnit {
public:
    // コンストラクタ
    Vector2Unit(const GSvector2& from, const GSvector2& to, float duration, std::function<void(const GSvector2&)> update_func);
    // 更新時の処理
    virtual void on_update(float progress) override;

private:
    GSvector2 from_;                                // 開始値
    GSvector2 to_;                                  // 終了値
    std::function<void(GSvector2)> update_func_;    // 更新時の処理
};

#endif
