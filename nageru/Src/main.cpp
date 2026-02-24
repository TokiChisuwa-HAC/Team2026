#include <GSgame.h>
#include "Scene/SceneManager.h"
#include "Scene/TitleScene/TitleScene.h"
#include "Scene/GamePlayScene/GamePlayScene.h"
#include "Scene/GamePlayScene/HitoshiScene/HitoshiScene.h"
#include "Scene/GamePlayScene/TokiScene/TokiScene.h"
#include "Scene/GamePlayScene/WataruScene/WataruScene.h"
#include "Scene/GamePlayScene/YoshinobuScene/YoshinobuScene.h"
#include <GSeffect.h>

// ゲームクラス
class MyGame : public gslib::Game {
public:
	MyGame() : gslib::Game{ 1920, 1080, false }{}

	// シーンマネージャー
	SceneManager scene_manager_;
	// 開始
	void start() override {
		// エフェクトの初期化
		gsInitEffect();
		// タイトルシーンの追加
		scene_manager_.add("TitleScene", new TitleScene());
		// ゲームプレイシーンの追加
		scene_manager_.add("GamePlayScene", new GamePlayScene());
		scene_manager_.add("HitoshiScene", new HitoshiScene());
		scene_manager_.add("TokiScene", new TokiScene());
		scene_manager_.add("WataruScene", new WataruScene());
		scene_manager_.add("YoshinobuScene", new YoshinobuScene());
		// タイトルシーンから開始
		scene_manager_.change("TitleScene");
	}

	// 更新
	void update(float delta_time) override {
		// シーンの更新
		scene_manager_.update(delta_time);
	}

	// 描画
	void draw() override {
		// シーンの描画
		scene_manager_.draw();
	}

	// 終了
	void end() override {
		// シーンの終了
		scene_manager_.end();
		// シーンの消去
		scene_manager_.clear();
		// エフェクトの終了
		gsFinishEffect();
	}
};

// メイン関数
int main() {
	return MyGame().run();
}