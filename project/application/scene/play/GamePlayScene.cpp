#include "GamePlayScene.h"

// system
#include "input/Input.h"
#include "manager/graphics/ModelManager.h"

#include "time/TimeManager.h"
// scene
#include "engine/scene/manager/SceneManager.h"
// app
#include "application/GameObject/component/collision/CollisionManager.h"
#include "../../ResultUI/Result.h"
#include "audio/Audio.h"
#include "manager/effect/PostProcessManager.h"


void GamePlayScene::Initialize()
{
	// 当たり判定マネージャーの初期化
	CollisionManager::GetInstance()->Initialize();
	// 衝突判定を2Dで実行
	CollisionManager::GetInstance()->SetCollisionDimension(CollisionDimension::Mode2D);
	// 衝突判定はXz平面で行う
	CollisionManager::GetInstance()->SetCollisionPlane(CollisionPlane::XZ);

	// Audioの読み込み
	Audio::GetInstance()->LoadWave("game_bgm", "bgm/game.wav", SoundGroup::BGM);
	Audio::GetInstance()->LoadWave("hit", "se/hit.wav", SoundGroup::SE);
	Audio::GetInstance()->LoadWave("level_up", "se/level_up.wav", SoundGroup::SE);
	Audio::GetInstance()->LoadWave("enemy_death", "se/enemy_death.wav", SoundGroup::SE);

	// 音声の再生
	Audio::GetInstance()->PlayWave("game_bgm", true);
	Audio::GetInstance()->SetVolume("game_bgm", 0.08f);
	Audio::GetInstance()->SetVolume("hit", 0.2f);

	// グレースケールをオフにする
	sceneManager_->GetPostProcessManager()->grayscaleEffect_->SetEnabled(false);

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

	// 敵出現範囲のAABB
	spawnRange_ = { {-50.0f,1.0f,-50.0f}, {50.0f,1.0f,50.0f} };

}

void GamePlayScene::Finalize()
{
	// グレースケールをオフにする
	sceneManager_->GetPostProcessManager()->grayscaleEffect_->SetEnabled(false);

	// 当たり判定マネージャーの終了
	CollisionManager::GetInstance()->Finalize();

	// Audioの停止
	Audio::GetInstance()->StopWave("game_bgm");
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
		TimeManager::GetInstance().SetTimeScale(1.0f);
		nextScene_ = true;
		fade_->Start(
			FadeType::FadeOut,
			2.0f
		);
		Result::GetInstance()->SetLifeTime(endTimer_);
		Result::GetInstance()->SetLevel(player_->GetPlayerData()->info.xp.level);

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
		enemyManager_->AddZombieEnemy(5, spawnRange_); // ゾンビ敵を5体追加
	}
	if (count == 1)
	{
		enemyManager_->AddZombieEnemy(3, spawnRange_); // ゾンビ敵を3体追加
		enemyManager_->AddRushEnemy(6, spawnRange_);  // ラッシュ敵を6体追加
	}
	if (count == 2)
	{
		enemyManager_->AddZombieEnemy(5, spawnRange_); // ゾンビ敵を5体追加
		enemyManager_->AddChargeEnemy(2, spawnRange_); // チャージ敵を2体追加
	}
	if (count == 3)
	{
		enemyManager_->AddBurstEnemy(3, spawnRange_); // バースト敵を3体追加
		enemyManager_->AddChargeEnemy(1, spawnRange_); // チャージ敵を1体追加
	}
	if (count == 4)
	{
		enemyManager_->AddZombieEnemy(10, spawnRange_); // ゾンビ敵を10体追加
	}
	if (count == 5)
	{
		enemyManager_->AddRushEnemy(20, spawnRange_);  // ラッシュ敵を20体追加
	}
	if (count == 6)
	{
		enemyManager_->AddZombieEnemy(6, spawnRange_); // ゾンビ敵を3体追加
		enemyManager_->AddRushEnemy(6, spawnRange_);  // ラッシュ敵を6体追加
		enemyManager_->AddBurstEnemy(2, spawnRange_); // バースト敵を2体追加
		enemyManager_->AddChargeEnemy(3, spawnRange_); // チャージ敵を3体追加
	}
	if (count == 7)
	{
		enemyManager_->AddBurstEnemy(10, spawnRange_); // バースト敵を10体追加
	}
}
