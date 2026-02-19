#ifndef VALUE_UNIT_H_
#define VALUE_UNIT_H_

#include "TweenUnit.h"

// 値(float型)を動かすためのTweenUnit
class ValueUnit : public TweenUnit {
public:
    // コンストラクタ
    ValueUnit(float from, float to, float duration, std::function<void(float)> update_func);
    // 更新時の処理
    virtual void on_update(float progress) override;

private:
    float from_;                                // 開始値
    float to_;                                  // 終了値
    std::function<void(float)> update_func_;    // 更新時の処理
};

#endif