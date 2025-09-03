#pragma once
// scene
#include "engine/scene/interface/BaseScene.h"

// app
#include "application//GameObject//Combatable//character//enemy//EnemyManager.h"

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

	// ゲームオブジェクト
	std::unique_ptr<EnemyManager> enemyManager_ = nullptr;

};

