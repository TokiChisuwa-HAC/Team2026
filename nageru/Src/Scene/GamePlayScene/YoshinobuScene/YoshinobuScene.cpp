#define GS_ENABLE_MESH_SHADOW
#include <GSstandard_shader.h>
#include <GSeffect.h>

#include "YoshinobuScene.h"
#include "Field/Field.h"
#include "Render/Camera/CameraTPS/CameraTPS.h"
#include "Render/Light/Light.h"
#include "Assets.h"

#include "Actor/Player/Player.h"
#include "Actor/Enemy/EnemyBasic/EnemyBasic.h"
#include "Actor/Enemy/EnemySpike/EnemySpike.h"

// 開始
void YoshinobuScene::start() {
    // 終了フラグを初期化
    is_end_ = false;
    // 次のシーン名を初期化
    next_scene_name_ = "TitleScene";
    // 視錐台カリングを有効にする
    gsEnable(GS_FRUSTUM_CULLING);
    // デフォルトシェーダーの初期化
    gsInitDefaultShader();
    // シャドウマップの作成
    static const GSuint shadow_map_size[] = { 2048, 2048 };
    gsCreateShadowMap(2, shadow_map_size, GS_TRUE);
    // シャドウマップを適用する距離(視点からの距離）
    gsSetShadowMapDistance(60.0f);
    // カスケードシャドウマップの分割位置を調整（デフォルトは0.5）
    gsSetShadowMapCascadeLamda(0.7f);
    // シャドウの濃さを設定(0.0:濃い～1.0:薄い)
    gsSetShadowMapAttenuation(0.0f);
    // シャドウマップのポリゴンオフセットを設定する
    gsEnableShadowMapPolygonOffset();
    gsSetShadowMapPolygonOffset(2.5f, 1.0f);
    // ライトマップの読み込み
    gsLoadLightmap(0, "Assets/Lightmap/Lightmap.txt");
    // リフレクションプローブの読み込み
    gsLoadReflectionProbe(0, "Assets/RefProbe/ReflectionProbe.txt");

    // スカイボックスメッシュの読み込み
    gsLoadMeshFixed(Mesh_Skybox, "Assets/Skybox/skydome.msh");
    // 描画用オクツリーの読み込み
    gsLoadOctree(Octree_Stage, "Assets/Octree/stage.oct");
    // 衝突判定用オクツリーの読み込み
    gsLoadOctree(Octree_Collider, "Assets/Octree/stage_collider.oct");

    // プレイヤーメッシュの読み込み
    gsLoadSkinMesh(Mesh_Player, "Assets/Player/player.mshb");
    // 基礎エネミーメッシュの読み込み
    gsLoadSkinMesh(Mesh_EnemyBasic, "Assets/Enemy/EnemyBasic/enemy_basic.mshb");
    // とげエネミーメッシュの読み込み
    gsLoadSkinMesh(Mesh_EnemySpike, "Assets/Enemy/EnemySpike/enemy_spike.mshb");

    // フィールドクラスの追加
    world_.add_field(new Field{ Octree_Stage, Octree_Collider, Mesh_Skybox });
    // カメラクラスの追加
    world_.add_camera(new CameraTPS{
        &world_, GSvector3{ 0.0f, 6.2f, -4.8f }, GSvector3{ 0.0f, 1.92, 0.0f } });
    // ライトクラスの追加
    world_.add_light(new Light{ &world_ });

    // プレイヤーの追加
    world_.add_actor(new Player{ &world_, GSvector3{ -30.0f, 0.125f, 0.0f } });
    // 基礎エネミーの追加
    world_.add_actor(new EnemyBasic{ &world_, GSvector3{ -30.0f, 0.125f, 10.0f } });
    // とげエネミーの追加
    world_.add_actor(new EnemySpike{ &world_, GSvector3{ -20.0f, 0.125f, 10.0f } });
}

// 更新
void YoshinobuScene::update(float delta_time) {
    // ワールドの更新
    world_.update(delta_time);
}

// 描画
void YoshinobuScene::draw() const {
    // ワールドの描画
    world_.draw();
}

// 終了しているか？
bool YoshinobuScene::is_end() const {
    // 終了フラグを返す
    return is_end_;
}

// 次のシーンを返す
std::string YoshinobuScene::next() const {
    return next_scene_name_;
}

// 終了
void YoshinobuScene::end() {
    // ワールドを消去
    world_.clear();
    // 再生中のエフェクトを停止する
    gsStopAllEffects();
    // メッシュの削除
    gsDeleteMesh(Mesh_Skybox);
    // スケルトンの削除

    // アニメーションの削除

    // オクトリーの削除
    gsDeleteOctree(Octree_Stage);
    gsDeleteOctree(Octree_Collider);
    // エフェクトの削除

}