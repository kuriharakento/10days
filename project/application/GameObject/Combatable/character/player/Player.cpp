#include "Player.h"

#include "application/GameObject/component/action/MoveComponent.h"
#include "application/GameObject/component/action/PlayerXPComponent.h"
#include "application/GameObject/component/action/LevelUpComponent.h"
#include "application/GameObject/component/action/PlayerAttackComponent.h"
#include "application/GameObject/component/base/ICollisionComponent.h"
#include "application/GameObject/component/collision/CollisionUtils.h"
#include "application/GameObject/component/collision/OBBColliderComponent.h"

#include "manager/editor/JsonEditorManager.h"

Player::~Player()
{
	data_.reset();
}

void Player::Initialize(Object3dCommon* object3dCommon, LightManager* lightManager)
{
	Character::Initialize(object3dCommon, lightManager);
	//初期位置を設定
	transform_.translate = { 0.0f, 1.0f, 0.0f };

	// 衝突判定コンポーネント
	AddComponent("OBBColliderComponent", std::make_unique<OBBColliderComponent>(this));

	// ステージデータの初期化
	data_ = std::make_unique<PlayerData>();

	data_->LoadJson("PlayerInfo.json");

	JsonEditorManager::GetInstance()->Register("PlayerInfo", data_);

	// 経験値
	data_->info.xp.calibrateS(/*λ*/0.6f, /*T*/180.0f, /*N* */10);
	data_->info.xp.recalc();

	// XPコンポーネントを追加
	AddComponent("PlayerXPComponent", std::make_unique<PlayerXPComponent>(data_.get()));
	// LvUPコンポーネントを追加
	AddComponent("LevelUpComponent", std::make_unique<LevelUpComponent>(data_.get()));
	// 移動コンポーネントを追加
	AddComponent("MoveComponent", std::make_unique<MoveComponent>(data_.get()));


#pragma region // 参考
	// 試しに腕を追加
	auto Rarm = std::make_unique<GameObject>(GameObjectTag::Character::PlayerRightArm);
	Rarm->Initialize(object3dCommon, lightManager);
	Rarm->SetModel("cube");
	Rarm->SetPosition(Vector3(3.0f, 0.0f, 0.0f));
	Rarm->AddComponent("OBBColliderComponent", std::make_unique<OBBColliderComponent>(Rarm.get()));
	Rarm->AddComponent("PlayerAttackComponent", std::make_unique<PlayerAttackComponent>(Rarm.get(), data_.get(), "R"));
	AddChild(GameObjectTag::Character::PlayerRightArm, std::move(Rarm));

	auto Larm = std::make_unique<GameObject>(GameObjectTag::Character::PlayerLeftArm);
	Larm->Initialize(object3dCommon, lightManager);
	Larm->SetModel("cube");
	Larm->SetPosition(Vector3(-3.0f, 0.0f, 0.0f));
	Larm->AddComponent("OBBColliderComponent", std::make_unique<OBBColliderComponent>(Larm.get()));
	Larm->AddComponent("PlayerAttackComponent", std::make_unique<PlayerAttackComponent>(Larm.get(), data_.get(), "L"));
	AddChild(GameObjectTag::Character::PlayerLeftArm, std::move(Larm));
#pragma endregion
}

void Player::Update()
{
	Character::Update();
}

void Player::Draw(CameraManager* camera)
{
	Character::Draw(camera);
}

void Player::CollisionSettings(ICollisionComponent* collider)
{
	// スイープ判定を仕様
	collider->SetUseSubstep(true);

	// 衝突時の処理を設定
	collider->SetOnEnter([this](GameObject* other) 
		{
		// 衝突した瞬間の処理

		});
	collider->SetOnStay([this](GameObject* other) 
		{
		// 衝突中の処理

		});
	collider->SetOnExit([this](GameObject* other) 
		{
		// 衝突が離れた時の処理

		});
}
