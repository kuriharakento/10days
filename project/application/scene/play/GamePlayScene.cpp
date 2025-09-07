#include "GamePlayScene.h"

// system
#include "input/Input.h"
// scene
#include "engine/scene/manager/SceneManager.h"
// app
#include "application/GameObject/component/collision/CollisionManager.h"


void GamePlayScene::Initialize()
{
	player_ = std::make_unique<Player>();
	player_->Initialize(
		sceneManager_->GetObject3dCommon(),
		sceneManager_->GetLightManager()
	);
	// エネミーマネージャーの初期化
	enemyManager_ = std::make_unique<EnemyManager>();
	enemyManager_->Initialize(sceneManager_->GetObject3dCommon(), sceneManager_->GetLightManager(), player_.get());
	enemyManager_->AddZombieEnemy(5); // ゾンビ敵を5体追加
	enemyManager_->AddRushEnemy(3);  // ラッシュ敵を3体追加
	enemyManager_->AddBurstEnemy(2); // バースト敵を2体追加
	enemyManager_->AddChargeEnemy(3); // チャージ敵を4体追加

}

void GamePlayScene::Finalize()
{
	
}

void GamePlayScene::Update()
{
	if (Input::GetInstance()->TriggerKey(DIK_SPACE))
	{
		sceneManager_->ChangeScene("TITLE");
	}
	player_->Update();
	// カメラこんな感じがいいかも
	sceneManager_->GetCameraManager()->GetActiveCamera()->SetRotate({ 0.78f,0,0 });
	sceneManager_->GetCameraManager()->GetActiveCamera()->SetTranslate(player_->GetPosition() + Vector3(0,50,-50));

	// エネミーマネージャーの更新
	enemyManager_->Update();
}

void GamePlayScene::Draw3D()
{
	player_->Draw(sceneManager_->GetCameraManager());
	// エネミーマネージャーの描画
	enemyManager_->Draw(sceneManager_->GetCameraManager());
}

void GamePlayScene::Draw2D()
{
	if (true)
	{
		for (auto&& spr : upgradeIcons_)
		{
			spr->Draw();
		}
	}
}

void GamePlayScene::ResisterSprite(std::string& path, Vector2 pos)
{
	auto sprite = std::make_unique<Sprite>();
	sprite->Initialize(sceneManager_->GetSpriteCommon(), path);
	sprite->SetPosition(pos);
	upgradeIcons_.push_back(std::move(sprite));
}
