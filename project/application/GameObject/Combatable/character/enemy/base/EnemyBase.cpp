#include "EnemyBase.h"

#include "../../../../component/action/EnemyMoveComponent.h"
#include "../../../../component/action/EnemyKnockbackComponent.h"

#include "application/GameObject/Combatable/character/player/Player.h"

void EnemyBase::Initialize(Object3dCommon* object3dCommon, LightManager* lightManager, float* moveSpeed, GameObject* target)
{
	Character::Initialize(object3dCommon, lightManager);

	// 移動速度を設定
	moveSpeed_ = moveSpeed;

	// ターゲットを設定
	target_ = target;

	AddComponent("EnemyMove", std::make_unique<EnemyMoveComponent>(moveSpeed_, target_));

}

void EnemyBase::Update()
{
	Character::Update();

	// 死亡処理
	if (hp_.base <= 0.0f)
	{
		isAlive_ = false;
	}

}

void EnemyBase::Draw(CameraManager* camera)
{
	Character::Draw(camera);
}

void EnemyBase::TakeAttack(GameObject* other, float knockbackValue)
{
	if (other->GetTag() == GameObjectTag::Character::PlayerRightArm ||
		other->GetTag() == GameObjectTag::Character::PlayerLeftArm)
	{
		if (!isInvincible_)
		{
			auto player = dynamic_cast<Player*>(target_);
			if (player->GetIsAttack())
			{
				auto combatable = dynamic_cast<CombatableObject*>(other);
				hp_.base -= combatable->GetAttackPower();

				auto direction = GetPosition() - Vector3{ other->GetWorldMatrix().m[3][0], other->GetWorldMatrix().m[3][1], other->GetWorldMatrix().m[3][2] };
				direction.y = 0;
				direction.Normalize();
				auto knockback = GetComponent<EnemyKnockbackComponent>();

				if (knockback)
				{
					knockback->StartKnokback(GetPosition(), GetPosition() + direction * player->GetKnockBack() * knockbackValue, 0.2f);
				}

				SetInvincible(0.5f);
			}
		}
	}
}
