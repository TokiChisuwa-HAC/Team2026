#ifndef FIELD_ACTOR_MANAGER_H_
#define FIELD_ACTOR_MANAGER_H_

#include "Actor/ActorManager.h"

class FieldActor;
class Ray;
class Line;
class BoundingSphere;
struct GSvector3;
struct GSplane;

// フィールドアクターマネージャー
class FieldActorManager : public ActorManager {
public:
    // レイとの衝突判定
    FieldActor* collide(
        const Ray& ray, float max_distance,
        GSvector3* intersect = nullptr, GSplane* plane = nullptr) const;
    // 線分との衝突判定
    FieldActor* collide(
        const Line& line,
        GSvector3* intersect = nullptr, GSplane* plane = nullptr) const;
    // 球体との衝突判定
    FieldActor* collide(
        const BoundingSphere& sphere,
        GSvector3* collided_center = nullptr) const;
};

#endif