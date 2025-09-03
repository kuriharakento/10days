#include "MoveComponent.h"
#include "application/GameObject/base/GameObject.h"
#include "math/MathUtils.h"
#include <cmath>

#include "base/Logger.h"
#include "math/Easing.h"
#include "time/TimerManager.h"
#include "time/Timer.h"

MoveComponent::MoveComponent()
{
}

void MoveComponent::Update(GameObject* owner)
{
    // タイマー更新
    deltaTime = 1.0f / 60.0f/*TimerManager::GetInstance().GetTimer("")*/;

    // 通常移動
    ProcessMovement(owner);

}

void MoveComponent::ProcessMovement(GameObject* owner)
{
    // 移動方向の取得
    Vector3 moveDirection = GetMovementDirection();
    hasMovementInput_ = moveDirection.Length() > 0.01f;

    // 移動処理
    if (hasMovementInput_)
    {
        moveDirection.NormalizeSelf(); // 正規化
        owner->SetPosition(owner->GetPosition() + moveDirection * moveSpeed_ * deltaTime/*TimerManager::GetInstance().GetTimer("")*/);

        // プレイヤーの向きを滑らかに変える
        UpdateRotation(owner, moveDirection);
    }
}

Vector3 MoveComponent::GetMovementDirection() const
{
    Vector3 direction(0, 0, 0);

    // WASDキーの入力を取得
    if (Input::GetInstance()->PushKey(DIK_W)) direction.z += 1.0f;
    if (Input::GetInstance()->PushKey(DIK_S)) direction.z -= 1.0f;
    if (Input::GetInstance()->PushKey(DIK_D)) direction.x += 1.0f;
    if (Input::GetInstance()->PushKey(DIK_A)) direction.x -= 1.0f;

    // 長さが0でなければ正規化
    if (direction.Length() > 0.01f)
    {
        direction.NormalizeSelf();
    }

    return direction;
}

void MoveComponent::UpdateRotation(GameObject* owner, const Vector3& direction)
{
    // 移動方向がある場合のみ向きを更新
    if (direction.Length() > 0.01f)
    {
        // 正規化された方向ベクトル
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
}
