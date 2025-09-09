#include "GamePlayScene.h"

// system
#include "input/Input.h"
#include "manager/graphics/ModelManager.h"

#include "time/TimeManager.h"
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

	ui_ = std::make_unique<PlayUI>(player_.get(), sceneManager_->GetSpriteCommon());
	ui_->Initialize();

	// ゾーンの生成
	zone_ = std::make_unique<Zone>();
	zone_->Initialize(sceneManager_->GetObject3dCommon(), sceneManager_->GetLightManager(), sceneManager_->GetCameraManager()->GetActiveCamera(), sceneManager_->GetPostProcessManager());

	ModelManager::GetInstance()->LoadModel("Ground");

	ground_ = std::make_unique<GameObject>();
	ground_->Initialize(sceneManager_->GetObject3dCommon(), sceneManager_->GetLightManager());
	ground_->SetModel("Ground");
	ground_->SetScale({ 300,1.0f,300 });

	// フェードの初期化
	fade_ = std::make_unique<Fade>();
	fade_->Initialize("./Resources/black.png", sceneManager_->GetSpriteCommon());
	fade_->Start(
		FadeType::FadeIn,
		2.0f
	);

}

void GamePlayScene::Finalize()
{
	
}

void GamePlayScene::Update()
{

#ifdef _DEBUG
	debugCamera_->Update();

	if (Input::GetInstance()->TriggerKey(DIK_TAB))
	{
		nextScene_ = true;
		fade_->Start(
			FadeType::FadeOut,
			2.0f
		);
	}
#endif // _DEBUG

	if (nextScene_ && !fade_->IsActive())
	{
		sceneManager_->ChangeScene("GAMECLEAR");
	}

	// カメラ
	sceneManager_->GetCameraManager()->GetActiveCamera()->SetRotate({ 0.68f,0,0 });
	sceneManager_->GetCameraManager()->GetActiveCamera()->SetTranslate(player_->GetPosition() + Vector3(0, 45, -58));

	if (!player_->IsAlive() || endTimer_ >= endTime_)
	{

		//========================↓↓↓ゲーム終了↓↓↓========================//

		nextScene_ = true;
		fade_->Start(
			FadeType::FadeOut,
			2.0f
		);

		//========================↑↑↑ゲーム終了↑↑↑========================//

	}
	else
	{

		//=========================↓↓↓ゲーム中↓↓↓=========================//

		CollisionManager::GetInstance()->UpdatePreviousPositions();

		player_->Update();
		ui_->Update();

		endTimer_ += TimeManager::GetInstance().GetDeltaTime();


		// エネミーマネージャーの更新
		enemyManager_->Update();
		// ゾーンの更新
		zone_->Update();
		ground_->Update();

		// 衝突判定開始
		CollisionManager::GetInstance()->CheckCollisions();

		if (timer_ < 15.0f)
		{
			timer_ += TimeManager::GetInstance().GetDeltaTime();
		}
		else if (spawnCount_ < 8)
		{
			Spawn(spawnCount_);
			++spawnCount_;
			timer_ = 0.0f;
		}

		//=========================↑↑↑ゲーム中↑↑↑=========================//

	}

	fade_->Update();
}

void GamePlayScene::Draw3D()
{
	player_->Draw(sceneManager_->GetCameraManager());
	// エネミーマネージャーの描画
	enemyManager_->Draw(sceneManager_->GetCameraManager());

	// ゾーンは行列のみ更新
	zone_->Draw(sceneManager_->GetCameraManager());

	ground_->Draw(sceneManager_->GetCameraManager());
}

void GamePlayScene::Draw2D()
{
	fade_->Draw();
	ui_->Draw();
}

void GamePlayScene::Spawn(int count)
{
	//enemyManager_->AddZombieEnemy(5); // ゾンビ敵を5体追加
	//enemyManager_->AddRushEnemy(3);  // ラッシュ敵を3体追加
	//enemyManager_->AddBurstEnemy(2); // バースト敵を2体追加
	//enemyManager_->AddChargeEnemy(3); // チャージ敵を4体追加
	if (count == 0)
	{
		enemyManager_->AddZombieEnemy(5); // ゾンビ敵を5体追加
	}
	if (count == 1)
	{
		enemyManager_->AddZombieEnemy(3); // ゾンビ敵を3体追加
		enemyManager_->AddRushEnemy(6);  // ラッシュ敵を6体追加
	}
	if (count == 2)
	{
		enemyManager_->AddZombieEnemy(5); // ゾンビ敵を5体追加
		enemyManager_->AddChargeEnemy(2); // チャージ敵を2体追加
	}
	if (count == 3)
	{
		enemyManager_->AddBurstEnemy(3); // バースト敵を3体追加
		enemyManager_->AddChargeEnemy(1); // チャージ敵を1体追加
	}
	if (count == 4)
	{
		enemyManager_->AddZombieEnemy(10); // ゾンビ敵を10体追加
	}
	if (count == 5)
	{
		enemyManager_->AddRushEnemy(20);  // ラッシュ敵を20体追加
	}
	if (count == 6)
	{
		enemyManager_->AddZombieEnemy(6); // ゾンビ敵を3体追加
		enemyManager_->AddRushEnemy(6);  // ラッシュ敵を6体追加
		enemyManager_->AddBurstEnemy(2); // バースト敵を2体追加
		enemyManager_->AddChargeEnemy(3); // チャージ敵を3体追加
	}
	if (count == 7)
	{
		enemyManager_->AddBurstEnemy(10); // バースト敵を10体追加
	}
}
