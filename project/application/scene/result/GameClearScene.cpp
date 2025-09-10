#include "GameClearScene.h"

// system
#include "input/Input.h"
#include "manager/graphics/ModelManager.h"

#include "time/TimeManager.h"
// scene
#include "engine/scene/manager/SceneManager.h"

void GameClearScene::Initialize()
{
	resultUI_ = std::make_unique<ResultUI>(sceneManager_->GetSpriteCommon());
	resultUI_->Initialize();

	// フェードの初期化
	fade_ = std::make_unique<Fade>();
	fade_->Initialize("./Resources/black.png", sceneManager_->GetSpriteCommon());
	fade_->Start(
		FadeType::FadeIn,
		2.0f
	);
}

void GameClearScene::Finalize()
{
}

void GameClearScene::Update()
{
	resultUI_->Update();

	if (Input::GetInstance()->TriggerKey(DIK_SPACE))
	{
		nextScene_ = true;
		fade_->Start(
			FadeType::FadeOut,
			2.0f
		);
	}

	if (nextScene_ && !fade_->IsActive())
	{
		sceneManager_->ChangeScene("TITLE");
	}

	fade_->Update();
}

void GameClearScene::Draw3D()
{
}

void GameClearScene::Draw2D()
{
	resultUI_->Draw();
	fade_->Draw();
}
