#include <GSgame.h>
#include "Scene/SceneManager.h"
#include "Scene/GamePlayScene/GamePlayScene.h"
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
		// ゲームプレイシーンの追加
		scene_manager_.add("GamePlayScene", new GamePlayScene());
		// ゲームプレイシーンから開始
		scene_manager_.change("GamePlayScene");
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