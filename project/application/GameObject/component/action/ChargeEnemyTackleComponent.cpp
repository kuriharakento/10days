#include "ChargeEnemyTackleComponent.h"


#include "application/GameObject/base/GameObject.h"
#include "application/GameObject/Combatable/character/base/Character.h"
#include "time/TimeManager.h"
#include <cmath>

ChargeEnemyTackleComponent::ChargeEnemyTackleComponent(float* moveSpeed, GameObject* target)
	: EnemyMoveComponent(moveSpeed, target)
	, chargeTimer_(0.0f)
	, tackleTimer_(0.0f)
	, isCharging_(false)
	, isTackling_(false)
{
}

void ChargeEnemyTackleComponent::Update(GameObject* owner)
{
	constexpr float approachDistance = 25.0f;   // タックル開始距離
	constexpr float chargeTime = 1.0f;         // 溜め時間（秒）
	constexpr float tackleSpeed = 50.0f;       // タックル時の速度
    constexpr float tackleDuration = 2.0f;     // タックル継続時間（秒）

	Vector3 ToTarget = target_->GetPosition() - owner->GetPosition();
	ToTarget.y = 0.0f;
	float distance = ToTarget.Length();

    if (!isCharging_ && !isTackling_)
    {
        if (distance > approachDistance)
        {
            // 通常追尾は親クラスのロジックを利用
            EnemyMoveComponent::Update(owner);
        } 
        else
        {
            // 溜め開始
            isCharging_ = true;
            chargeTimer_ = 0.0f;
        }
    } 
    else if (isCharging_)
    {
        // 溜め中
        chargeTimer_ += TimeManager::GetInstance().GetDeltaTime();
        // 上下に細かく揺れる
		float bobbingAmplitude = 0.5f; // 揺れの振幅
		float bobbingFrequency = 25.0f; // 揺れの周波数
		Vector3 currentPosition = owner->GetPosition();
		currentPosition.y = 1.0f + bobbingAmplitude * sinf(bobbingFrequency * chargeTimer_);
		owner->SetPosition(currentPosition);

        if (chargeTimer_ >= chargeTime)
        {
			// y座標をリセット
			currentPosition.y = 1.0f;
			owner->SetPosition(currentPosition);

            // タックル開始
            isCharging_ = false;
            isTackling_ = true;
            tackleTimer_ = 0.0f;
            tackleDirection_ = ToTarget.Normalize();
        }
    }
    else if (isTackling_)
    {
        // タックル移動
        Vector3 movement = tackleDirection_ * tackleSpeed * TimeManager::GetInstance().GetDeltaTime();
        owner->SetPosition(owner->GetPosition() + movement);

        float targetRotationY = atan2f(tackleDirection_.x, tackleDirection_.z);
        Vector3 currentRotation = owner->GetRotation();
        owner->SetRotation({ currentRotation.x, targetRotationY, currentRotation.z });

        // タックル時間計測
        tackleTimer_ += TimeManager::GetInstance().GetDeltaTime();
        if (tackleTimer_ >= tackleDuration)
        {
            // タックル終了
            isTackling_ = false;
            tackleTimer_ = 0.0f;
        }

    }

}

