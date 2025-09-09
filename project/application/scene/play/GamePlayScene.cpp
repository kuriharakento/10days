#include "GamePlayScene.h"

// system
#include "input/Input.h"
#include "manager/graphics/TextureManager.h"
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
	//enemyManager_->AddZombieEnemy(5); // ゾンビ敵を5体追加
	//enemyManager_->AddRushEnemy(3);  // ラッシュ敵を3体追加
	//enemyManager_->AddBurstEnemy(2); // バースト敵を2体追加
	//enemyManager_->AddChargeEnemy(3); // チャージ敵を4体追加

	ResisterSprite("Resources/icon/ADIcon.png", {300,200});
	ResisterSprite("Resources/icon/ASIcon.png", { 590,200 });
	ResisterSprite("Resources/icon/MSIcon.png", { 880,200 });
	ResisterSprite("Resources/icon/KBIcon.png", { 450,500 });
	ResisterSprite("Resources/icon/HEALIcon.png", { 730,500 });

	// ゾーンの生成
	zone_ = std::make_unique<Zone>();
	zone_->Initialize(sceneManager_->GetObject3dCommon(), sceneManager_->GetLightManager(), sceneManager_->GetCameraManager()->GetActiveCamera(), sceneManager_->GetPostProcessManager());

	ModelManager::GetInstance()->LoadModel("Ground");
	obj_ = std::make_unique<GameObject>();
	obj_->Initialize(sceneManager_->GetObject3dCommon(), sceneManager_->GetLightManager());
	obj_->SetModel("Ground");
	obj_->SetScale({ 300,1.0f,300 });

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
	debugCamera_->Update();

	if (Input::GetInstance()->TriggerKey(DIK_TAB))
	{
		nextScene_ = true;
		fade_->Start(
			FadeType::FadeOut,
			2.0f
		);
	}
	if (nextScene_ && !fade_->IsActive())
	{
		sceneManager_->ChangeScene("GAMECLEAR");
	}

	CollisionManager::GetInstance()->UpdatePreviousPositions();

	player_->Update();
	for (auto&& spr : upgradeIcons_)
	{
		spr->Update();
	}
	// カメラこんな感じがいいかも
	sceneManager_->GetCameraManager()->GetActiveCamera()->SetRotate({ 0.78f,0,0 });
	sceneManager_->GetCameraManager()->GetActiveCamera()->SetTranslate(player_->GetPosition() + Vector3(0,50,-50));

	
	// エネミーマネージャーの更新
	enemyManager_->Update();
  
	// ゾーンの更新
	zone_->Update();

	obj_->Update();

	fade_->Update();

	// 衝突判定開始
	CollisionManager::GetInstance()->CheckCollisions();

	if (timer_ < 10.0f)
	{
		timer_ += TimeManager::GetInstance().GetDeltaTime();;
	}
	else if(spawnCount_ < 3)
	{
		enemyManager_->AddZombieEnemy(15); // ゾンビ敵を15体追加
		++spawnCount_;
	}
}

void GamePlayScene::Draw3D()
{
	player_->Draw(sceneManager_->GetCameraManager());
	// エネミーマネージャーの描画
	enemyManager_->Draw(sceneManager_->GetCameraManager());

	// ゾーンは行列のみ更新
	zone_->Draw(sceneManager_->GetCameraManager());

	obj_->Draw(sceneManager_->GetCameraManager());
}

void GamePlayScene::Draw2D()
{
	if (player_->GetIsUpgrade())
	{
		for (auto&& spr : upgradeIcons_)
		{
			spr->Draw();
		}
	}

	fade_->Draw();
}

void GamePlayScene::ResisterSprite(const std::string& path, Vector2 pos)
{
	TextureManager::GetInstance()->LoadTexture(path);
	auto sprite = std::make_unique<Sprite>();
	sprite->Initialize(sceneManager_->GetSpriteCommon(), path);
	sprite->SetPosition(pos);
	sprite->SetAnchorPoint({0.5f,0.5f});
	upgradeIcons_.push_back(std::move(sprite));
}
