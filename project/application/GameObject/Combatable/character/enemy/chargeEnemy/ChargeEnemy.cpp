#include "ChargeEnemy.h"

#include "../../../../component/collision/OBBColliderComponent.h"
#include "../../../../component/action/ChargeEnemyTackleComponent.h"

void ChargeEnemy::Initialize(Object3dCommon* object3dCommon, LightManager* lightManager, float* moveSpeed, GameObject* target)
{
	EnemyBase::Initialize(object3dCommon, lightManager, moveSpeed, target);

	// モデルの設定
	SetModel("cube");
	// 初期位置設定
	SetPosition(Vector3(0.0f, GetScale().y, 10.0f));
	// HP
	maxHp_.base = 80.0f; // チャージエネミーの最大HP
	hp_.base = maxHp_.base; // 現在のHPを最大HPに設定


	// OBBコライダーコンポーネントを追加
	AddComponent("OBBColliderComponent", std::make_unique<OBBColliderComponent>(this));
	// タックルコンポーネントを追加
	AddComponent("ChargeEnemyTackleComponent", std::make_unique<ChargeEnemyTackleComponent>(moveSpeed, target));

}

void ChargeEnemy::Update()
{
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
			// 衝突した瞬間の処理
			if (other->GetTag() == GameObjectTag::Weapon::PlayerBullet)
			{
				auto combatable = dynamic_cast<CombatableObject*>(other);
				hp_.base -= combatable->GetAttackPower();
				// ノックバック
				auto direction = GetPosition() - other->GetPosition();
				direction.y = 0; // 水平面での向きに制限
				direction.Normalize();
				SetPosition(GetPosition() + direction * 0.5f); // ノックバック量を調整
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
