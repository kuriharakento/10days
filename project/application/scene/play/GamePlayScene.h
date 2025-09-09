#pragma once
// scene
#include "engine/scene/interface/BaseScene.h"
// app
#include "application/GameObject/Combatable/character/player/Player.h"
#include "application//GameObject//Combatable//character//enemy//EnemyManager.h"
#include "application/GameObject/zone/Zone.h"
#include "camerawork/debug/DebugCamera.h"
#include "../../PlayUI/PlayUI.h"

class GamePlayScene : public BaseScene
{
public:
	//初期化
	void Initialize() override;
	//終了
	void Finalize() override;
	//更新
	void Update() override;
	//描画
	void Draw3D() override;
	void Draw2D() override;

private:
	// デバッグカメラ
	std::unique_ptr<DebugCamera> debugCamera_;

	std::unique_ptr<Player> player_;

	// ゲームオブジェクト
	std::unique_ptr<EnemyManager> enemyManager_ = nullptr;

	// ゾーン
	std::unique_ptr<Zone> zone_;

	std::unique_ptr<PlayUI> ui_;

	std::unique_ptr<GameObject> ground_;

	float timer_ = 0.0f;
	int spawnCount_ = 0;
};

