#include "GamePlayScene.h"

// system
#include "input/Input.h"
// scene
#include "engine/scene/manager/SceneManager.h"


void GamePlayScene::Initialize()
{
	player_ = std::make_unique<Player>();
	player_->Initialize(
		sceneManager_->GetObject3dCommon(),
		sceneManager_->GetLightManager()
	);
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
}

void GamePlayScene::Draw3D()
{
	player_->Draw(sceneManager_->GetCameraManager());
}

void GamePlayScene::Draw2D()
{

}
