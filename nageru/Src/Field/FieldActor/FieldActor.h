#ifndef FIELD_ACTOR_H_
#define FIELD_ACTOR_H_

#include "Actor/Actor.h"

// 前方宣言
class Ray;
class Line;
class BoundingSphere;

// フィールド用のアクタークラス
class FieldActor : public Actor {
public:
    // 描画
    virtual void draw() const override;

    // レイとの衝突判定
    virtual bool collide(
        const Ray& ray, float max_distance,
        GSvector3* intersect = nullptr, GSplane* plane = nullptr) const;
    // 線分との衝突判定
    virtual bool collide(
        const Line& line,
        GSvector3* intersect = nullptr, GSplane* plane = nullptr) const;
    // 球体との衝突判定
    virtual bool collide(
        const BoundingSphere& sphere,
        GSvector3* intersect = nullptr) const;

protected:
    // メッシュ
    GSuint          mesh_{ 0 };
    // メッシュコライダ
    GSuint          mesh_collider_{ 0 };
};

#endif