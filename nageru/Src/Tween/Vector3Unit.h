#ifndef VECTOR3_UNIT_H_
#define VECTOR3_UNIT_H_

#include "TweenUnit.h"

// Vector3型を動かすためのTweenUnit
class Vector3Unit : public TweenUnit {
public:
    // コンストラクタ
    Vector3Unit(const GSvector3& from, const GSvector3& to, float duration, std::function<void(const GSvector3&)> update_func);
    // 更新時の処理
    virtual void on_update(float progress) override;

private:
    GSvector3 from_;                                // 開始値
    GSvector3 to_;                                  // 終了値
    std::function<void(GSvector3)> update_func_;    // 更新時の処理
};

#endif
