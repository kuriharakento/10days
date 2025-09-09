#include "RushEnemy.h"

#include "../../../../component/collision/OBBColliderComponent.h"
#include "../../../../component/action/EnemyKnockbackComponent.h"

void RushEnemy::Initialize(Object3dCommon* object3dCommon, LightManager* lightManager, float* moveSpeed, GameObject* target)
{
	EnemyBase::Initialize(object3dCommon, lightManager, moveSpeed, target);

	// モデルの設定
	SetModel("cube");

	// 初期位置設定
	SetPosition(Vector3(0.0f, GetScale().y, 10.0f));
	// HP
	maxHp_.base = 10.0f; // ラッシュエネミーの最大HP
	hp_.base = maxHp_.base; // 現在のHPを最大HPに設定

	// OBBコライダーコンポーネントを追加
	AddComponent("OBBColliderComponent", std::make_unique<OBBColliderComponent>(this));

	// ノックバックコンポーネントを追加
	AddComponent("EnemyKnockbackComponent", std::make_unique<EnemyKnockbackComponent>());

}

void RushEnemy::Update()
{
	auto knockback = GetComponent<EnemyKnockbackComponent>();
	if (knockback && knockback->IsKnockback())
	{
		knockback->Update(this);
		// ノックバック中は他の移動処理をスキップ
		return;
	}

	EnemyBase::Update();
}

void RushEnemy::Draw(CameraManager* camera)
{
	EnemyBase::Draw(camera);
}

void RushEnemy::CollisionSettings(ICollisionComponent* collider)
{
	// 衝突時の処理を設定
	collider->SetOnEnter([this](GameObject* other)
		{
			if (other->GetTag() == GameObjectTag::Character::PlayerRightArm ||
				other->GetTag() == GameObjectTag::Character::PlayerLeftArm)
			{
				auto combatable = dynamic_cast<CombatableObject*>(other);
				hp_.base -= combatable->GetAttackPower();

				auto direction = GetPosition() - other->GetPosition();
				direction.y = 0;
				direction.Normalize();
				auto knockback = GetComponent<EnemyKnockbackComponent>();

				if (knockback)
				{
					knockback->StartKnokback(GetPosition(), GetPosition() + direction * 0.5f, 0.2f);
				}

				SetInvincible(0.5f);
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
