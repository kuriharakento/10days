#include "ChargeEnemy.h"

#include "../../../../component/collision/OBBColliderComponent.h"
#include "../../../../component/action/EnemyKnockbackComponent.h"
#include "../../../../component/action/ChargeEnemyTackleComponent.h"

#include "application/GameObject/Combatable/character/player/Player.h"

void ChargeEnemy::Initialize(Object3dCommon* object3dCommon, LightManager* lightManager, float* moveSpeed, GameObject* target)
{
	EnemyBase::Initialize(object3dCommon, lightManager, moveSpeed, target);

	// モデルの設定
	SetModel("charge");
	// 初期位置設定
	SetPosition(Vector3(0.0f, GetScale().y, 10.0f));
	// HP
	maxHp_.base = 10.0f; // チャージエネミーの最大HP
	hp_.base = maxHp_.base; // 現在のHPを最大HPに設定


	// OBBコライダーコンポーネントを追加
	AddComponent("OBBColliderComponent", std::make_unique<OBBColliderComponent>(this));

	// ノックバックコンポーネントを追加
	AddComponent("EnemyKnockbackComponent", std::make_unique<EnemyKnockbackComponent>());

	// タックルコンポーネントを追加
	AddComponent("ChargeEnemyTackleComponent", std::make_unique<ChargeEnemyTackleComponent>(moveSpeed, target));

}

void ChargeEnemy::Update()
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

void ChargeEnemy::Draw(CameraManager* camera)
{
	EnemyBase::Draw(camera);
}

void ChargeEnemy::CollisionSettings(ICollisionComponent* collider)
{
	// 衝突時の処理を設定
	collider->SetOnEnter([this](GameObject* other)
		{
			TakeAttack(other, 1.0f);
		});

	collider->SetOnStay([this](GameObject* other)
		{
			// 衝突中の処理
			TakeAttack(other, 1.0f);
		});
	collider->SetOnExit([this](GameObject* other)
		{
			// 衝突が離れた時の処理
		});
}
