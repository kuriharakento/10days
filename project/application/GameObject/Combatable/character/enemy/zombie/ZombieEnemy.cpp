#include "ZombieEnemy.h"

#include "../../../../component/collision/OBBColliderComponent.h"


#include "input/Input.h"

void ZombieEnemy::Initialize(Object3dCommon* object3dCommon, LightManager* lightManager, float* moveSpeed, GameObject* target)
{
	EnemyBase::Initialize(object3dCommon, lightManager, moveSpeed, target);
	// モデルの設定
	SetModel("cube");
	// 初期位置設定
	SetPosition(Vector3(0.0f, GetScale().y, 10.0f));
	// HP
	maxHp_.base = 50.0f; // ゾンビの最大HP
	hp_.base = maxHp_.base; // 現在のHPを最大HPに設定

	// OBBコライダーコンポーネントを追加
	AddComponent("OBBColliderComponent", std::make_unique<OBBColliderComponent>(this));
}

void ZombieEnemy::Update()
{
#ifdef _DEBUG

	if (Input::GetInstance()->TriggerKey(DIK_K))
	{
		// ノックバックテスト
		auto direction = Vector3(0, 0, -1); // 前方にノックバック
		direction.Normalize();
		knockbackStartPos_ = GetPosition();
		knockbackTargetPos_ = GetPosition() + direction * 2.5f; // 0.5fはノックバックの距離
		knockbackElapsed_ = 0.0f;
		isKnockback_ = true;
	}
	

#endif // _DEBUG


	if (isKnockback_) 
	{
		knockbackElapsed_ += /* deltaTime */ 0.016f; // フレーム時間を渡す場合は適宜修正
		float t = std::min(knockbackElapsed_ / knockbackDuration_, 1.0f);
		Vector3 newPos = knockbackStartPos_ * (1.0f - t) + knockbackTargetPos_ * t;
		SetPosition(newPos);
		if (t >= 1.0f)
		{
			isKnockback_ = false;
		}
	}

	EnemyBase::Update();
}

void ZombieEnemy::Draw(CameraManager* camera)
{
	EnemyBase::Draw(camera);
}

void ZombieEnemy::CollisionSettings(ICollisionComponent* collider)
{
	// 衝突時の処理を設定
	collider->SetOnEnter([this](GameObject* other) 
	{
		// 衝突した瞬間の処理
		if (other->GetTag() == GameObjectTag::Weapon::PlayerBullet)
		{
			auto combatable = dynamic_cast<CombatableObject*>(other);
			hp_.base -= combatable->GetAttackPower();

			// ノックバック
			auto direction = GetPosition() - other->GetPosition();
			direction.y = 0;
			direction.Normalize();
			knockbackStartPos_ = GetPosition();
			knockbackTargetPos_ = GetPosition() + direction * 0.5f; // 0.5fはノックバックの距離
			knockbackElapsed_ = 0.0f;
			isKnockback_ = true;

			// 無敵時間を設定
			SetInvincible(0.5f); // 0.5秒の無敵時間
		}

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
