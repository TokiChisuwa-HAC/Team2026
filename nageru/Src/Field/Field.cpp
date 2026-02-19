#include "Field.h"
#include "Math/Ray.h"
#include "Math/Line.h"
#include "Math/Collider/BoundingSphere.h"
#include "FieldActor/FieldActor.h"

// コンストラクタ
Field::Field(GSuint octree, GSuint collider, GSuint skybox) :
    octree_{ octree }, collider_{ collider }, skybox_{ skybox } {

}

// デストラクタ
Field::~Field() {
    clear();
}

// 更新
void Field::update(float delta_time) {
    actors_.update(delta_time);
    actors_.remove();
}

// 描画
void Field::draw() const {
    // スカイボックスの描画
    gsDrawSkybox(skybox_);
    // オクトリーの描画
    gsDrawOctree(octree_);
    // フィーsルドアクターの描画
    draw_actors();
}

// フィールドアクター用の描画
void Field::draw_actors() const {
    actors_.draw();
}

// レイとの衝突判定
bool Field::collide(const Ray& ray, float max_distance, GSvector3* intersect, GSplane* plane, Actor** actor) const {
    Line line;
    line.start = ray.position;
    line.end = ray.position + (ray.direction.normalized() * max_distance);
    return collide(line, intersect, plane, actor);
}

// 線分との衝突判定
bool Field::collide(const Line& line, GSvector3* intersect, GSplane* plane, Actor** actor) const {
    // オクトリーとの交差判定
    GSvector3 octree_intersection_point;
    GSplane octree_intersection_plane;
    bool intersection_octree = gsOctreeCollisionLine(
        gsGetOctree(collider_),
        &line.start, &line.end, &octree_intersection_point, &octree_intersection_plane) == GS_TRUE;
    // アクターとの交差判定
    GSvector3 actor_intersection_point;
    GSplane actor_intersection_plane;
    Actor* intersection_actor = actors_.collide(line, &actor_intersection_point, &actor_intersection_plane);
    // オクトリーとアクター両方に衝突しているか？
    if (intersection_octree && intersection_actor != nullptr) {
        // 始点と交点の距離を計算する
        float distance_to_actor = line.start.distance(actor_intersection_point);
        float distance_to_octree = line.start.distance(octree_intersection_point);
        if (distance_to_octree < distance_to_actor) {
            // オクトリーの方が近い場合はアクターに衝突していないことにする
            intersection_actor = nullptr;
        }
        else {
            // アクターの方が近い場合オクトリーに衝突していないことにする
            intersection_octree = false;
        }
    }
    // オクトリーに衝突しているか？
    if (intersection_octree) {
        if (intersect != nullptr) {
            *intersect = octree_intersection_point;
        }
        if (plane != nullptr) {
            *plane = octree_intersection_plane;
        }
        if (actor != nullptr) {
            *actor = nullptr;
        }
        return true;
    }
    // アクターに衝突しているか？
    if (intersection_actor) {
        if (intersect != nullptr) {
            *intersect = actor_intersection_point;
        }
        if (plane != nullptr) {
            *plane = actor_intersection_plane;
        }
        if (actor != nullptr) {
            *actor = intersection_actor;
        }
        return true;
    }
    // 何も衝突しなかった
    return false;
}

// 球体との衝突判定
bool Field::collide(const BoundingSphere& sphere, GSvector3* center, Actor** actor) const {
    // 衝突判定後の半径
    GSvector3 collided_center = sphere.center;
    // フィールド用のアクターとの衝突判定を行う
    Actor* collided_actor = actors_.collide(sphere, &collided_center);
    // オクトリーと衝突判定を行う
    bool collided_octree = gsOctreeCollisionSphere(
        gsGetOctree(collider_),
        &collided_center, sphere.radius, &collided_center) == GS_TRUE;
    if (center != nullptr) {
        *center = collided_center;
    }
    if (actor != nullptr) {
        *actor = collided_actor;
    }
    // オクトリーかアクターどちらかに衝突していたか？
    return collided_octree || collided_actor != nullptr;
}

// 重力値を取得
float Field::gravity() const {
    return gravity_;
}

// フィールド用アクターの追加
void Field::add(Actor* actor) {
    actors_.add(actor);
}

// フィールド用アクターの消去
void Field::clear() {
    actors_.clear();
}