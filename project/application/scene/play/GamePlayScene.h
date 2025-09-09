#pragma once
// scene
#include "engine/scene/interface/BaseScene.h"

#include "engine/graphics/2d/Sprite.h"
// app
#include "application/GameObject/Combatable/character/player/Player.h"
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
	void ResisterSprite(const std::string& path, Vector2 pos = {0,0});

private:
	std::unique_ptr<Player> player_;

	// ゲームオブジェクト
	std::unique_ptr<EnemyManager> enemyManager_ = nullptr;

	std::vector<std::unique_ptr<Sprite>> upgradeIcons_;

	std::unique_ptr<GameObject> obj_;

	float timer_ = 0.0f;
	int spawnCount_ = 0;
};

