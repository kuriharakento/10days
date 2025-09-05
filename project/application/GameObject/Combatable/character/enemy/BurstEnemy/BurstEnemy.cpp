#include "BurstEnemy.h"

#include "../../../../component/collision/OBBColliderComponent.h"

#ifdef _DEBUG

#include "input/Input.h"

#endif // _DEBUG


void BurstEnemy::Initialize(Object3dCommon* object3dCommon, LightManager* lightManager, float* moveSpeed, GameObject* target)
{
	EnemyBase::Initialize(object3dCommon, lightManager, moveSpeed, target);

	// モデルの設定
	SetModel("cube");
	// HP
	maxHp_.base = 30.0f; // バーストエネミーの最大HP
	hp_.base = maxHp_.base; // 現在のHPを最大HPに設定

	// 分裂後に生きている状態にする
	isAlive_ = true;

	// OBBコライダーコンポーネントを追加
	AddComponent("OBBColliderComponent", std::make_unique<OBBColliderComponent>(this));

}

void BurstEnemy::Update()
{
	EnemyBase::Update();

#ifdef _DEBUG

	// デバッグ用：キー入力で死亡
	if (Input::GetInstance()->TriggerKey(DIK_1))
	{
		hp_.base = 0.0f; // HPを0にして死亡させる
	}

#endif // _DEBUG


}

void BurstEnemy::Draw(CameraManager* camera)
{
	EnemyBase::Draw(camera);
}

void BurstEnemy::CollisionSettings(ICollisionComponent* collider)
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
