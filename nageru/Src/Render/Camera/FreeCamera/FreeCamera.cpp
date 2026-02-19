#include "FreeCamera.h"
#include "World/IWorld.h"

// プレーヤーからの相対座標
const GSvector3 PlayerOffset{ 0.0f, 3.0f, -5.0f };
// カメラの注視点の補正値
const GSvector3 ReferencePointOffset{ 0.0f, 1.7f, 0.0f };

FreeCamera::FreeCamera(IWorld* world) {
    world_ = world;
    name_ = "Camera";
    tag_ = "CameraTag";
}

void FreeCamera::draw() const {
    GSvector3 eye = world_->free_camera_eye();
    GSvector3 at = world_->free_camera_at();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(
        eye.x, eye.y, eye.z,
        at.x, at.y, at.z,
        0.0f, 1.0f, 0.0f
    );
}