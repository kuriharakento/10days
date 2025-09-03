#include "EnemyMoveComponent.h"

#include "application/GameObject/base/GameObject.h"
#include "application/GameObject/Combatable/character/base/Character.h"
#include "time/TimeManager.h"

EnemyMoveComponent::EnemyMoveComponent(float* speed, GameObject* target)
	: speed_(*speed)
	, target_(target)
{
}

void EnemyMoveComponent::Update(GameObject* owner)
{
	// ターゲットへの向きを計算
	Vector3 direction = target_->GetPosition() - owner->GetPosition();
	direction.y = 0; // 水平面での向きに制限

	direction.Normalize();
	// 移動量を計算
	Vector3 movement = (direction * speed_) * TimeManager::GetInstance().GetDeltaTime();

	// 位置を更新
	owner->SetPosition(owner->GetPosition() + movement);

	// 向きをターゲットに向ける
	owner->SetRotation(Vector3(0, atan2f(direction.x, direction.z) * (180.0f / 3.14159f), 0));

}
