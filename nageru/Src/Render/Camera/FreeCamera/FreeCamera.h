#ifndef CAMERA_H_
#define CAMERA_H_

#include "Actor/Actor.h"

class FreeCamera : public Actor {
public:
    // コンストラクタ
    FreeCamera(IWorld* world);
    // 描画
    virtual void draw() const override;
};

#endif

