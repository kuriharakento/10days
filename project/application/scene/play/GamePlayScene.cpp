#include "GamePlayScene.h"

// system
#include "input/Input.h"
// scene
#include "engine/scene/manager/SceneManager.h"
// app
#include "application/GameObject/component/collision/CollisionManager.h"


void GamePlayScene::Initialize()
{
	// 当たり判定マネージャーの初期化
	CollisionManager::GetInstance()->Initialize();
	// 衝突判定を2Dで実行
	CollisionManager::GetInstance()->SetCollisionDimension(CollisionDimension::Mode2D);
	// 衝突判定はXz平面で行う
	CollisionManager::GetInstance()->SetCollisionPlane(CollisionPlane::XZ);

	// デバッグカメラを起動
	debugCamera_ = std::make_unique<DebugCamera>();
	debugCamera_->Initialize(sceneManager_->GetCameraManager()->GetActiveCamera());
	debugCamera_->Start();

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

	// ゾーンの生成
	zone_ = std::make_unique<Zone>();
	zone_->Initialize(sceneManager_->GetObject3dCommon(), sceneManager_->GetLightManager(), sceneManager_->GetCameraManager()->GetActiveCamera(), sceneManager_->GetPostProcessManager());
}

void GamePlayScene::Finalize()
{
	
}

void GamePlayScene::Update()
{
	debugCamera_->Update();

	if (Input::GetInstance()->TriggerKey(DIK_SPACE))
	{
		sceneManager_->ChangeScene("TITLE");
	}

	CollisionManager::GetInstance()->UpdatePreviousPositions();

	player_->Update();

	// エネミーマネージャーの更新
	enemyManager_->Update();

	// ゾーンの更新
	zone_->Update();

	CollisionManager::GetInstance()->CheckCollisions();
}

void GamePlayScene::Draw3D()
{
	player_->Draw(sceneManager_->GetCameraManager());
	// エネミーマネージャーの描画
	enemyManager_->Draw(sceneManager_->GetCameraManager());
	// ゾーンは行列のみ更新
	zone_->Draw(sceneManager_->GetCameraManager());
}

void GamePlayScene::Draw2D()
{

}
