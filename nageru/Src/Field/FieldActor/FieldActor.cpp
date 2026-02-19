#include "FieldActor.h"
#include "Math/Line.h"
#include "Math/Ray.h"
#include "Math/Collider/BoundingSphere.h"

// 描画
void FieldActor::draw() const {
    // メッシュの描画
    glPushMatrix();
    glMultMatrixf(transform_.localToWorldMatrix());
    gsDrawMesh(mesh_);
    glPopMatrix();
}

// レイとの衝突判定
bool FieldActor::collide(const Ray& ray, float max_distance, GSvector3* intersect, GSplane* plane) const {
    // レイのパラメータを線分に変換
    Line line;
    line.start = ray.position;
    line.end = ray.position + (ray.direction.normalized() * max_distance);
    // 線分との衝突判定を行う
    return collide(line, intersect, plane);
}

// 線分との衝突判定
bool FieldActor::collide(const Line& line, GSvector3* intersect, GSplane* plane) const {
    // ローカル座標系の線分を求める
    Line local_line;
    local_line.start = transform_.inverseTransformPoint(line.start);
    local_line.end = transform_.inverseTransformPoint(line.end);
    // メッシュと線分が交差したか？
    if (gsCollisionMeshAndLine(gsGetMesh(mesh_collider_), &local_line.start, &local_line.end, intersect, plane) == GS_TRUE) {
        if (intersect != nullptr) {
            // 交点をワールド座標系に変換
            *intersect = transform_.transformPoint(*intersect);
        }
        if (plane != nullptr) {
            // 平面の方程式をワールド座標系に変換
            const GSmatrix4& local_to_world_matrix = transform_.localToWorldMatrix();
            gsPlaneTransform(plane, plane, &local_to_world_matrix);
        }
        return true;
    }
    return false;
}

// 球体との衝突判定
bool FieldActor::collide(const BoundingSphere& sphere, GSvector3* collided_center) const {
    // ローカル座標系の球体を求める
    BoundingSphere local_sphere;
    local_sphere.center = transform_.inverseTransformPoint(sphere.center);
    local_sphere.radius = sphere.radius;
    // メッシュと球体が衝突したか？
    GSvector3 local_collided_center;
    if (gsCollisionMeshAndSphere(gsGetMesh(mesh_collider_), &local_sphere.center, local_sphere.radius, &local_collided_center) == GS_TRUE) {
        if (collided_center != nullptr) {
            // 衝突判定後の中心座標をワールド座標系に変換
            *collided_center = transform_.transformPoint(local_collided_center);
        }
        return true;
    }
    return false;
}