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

	direction = direction.Normalize();
	// 移動量を計算
	Vector3 movement = (direction * speed_) * TimeManager::GetInstance().GetDeltaTime();

	// 位置を更新
	owner->SetPosition(owner->GetPosition() + movement);

    //正規化された方向ベクトル
       Vector3 normalizedDir = direction;
    normalizedDir.NormalizeSelf();

    // Y軸回りの目標回転角度を計算（atan2を使用）
    float targetRotationY = atan2f(normalizedDir.x, normalizedDir.z);

    // 現在の回転を取得
    Vector3 currentRotation = owner->GetRotation();

    // Y軸の回転のみ、最短経路で補間
    float easedRotationY = MathUtils::LerpAngle(
        currentRotation.y,
        targetRotationY,
        0.2f // 補間速度（回避中は少し速めに）
    );

    // 回転を更新
    owner->SetRotation({ currentRotation.x, easedRotationY, currentRotation.z });

}
