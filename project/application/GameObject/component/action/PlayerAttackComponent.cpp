#include "PlayerAttackComponent.h"
#include "application/GameObject/base/GameObject.h"
#include "math/MathUtils.h"
#include <cmath>

#include "time/TimeManager.h"

PlayerAttackComponent::PlayerAttackComponent(GameObject* owner, PlayerData* data, const char* LR)
{
	owner_ = owner;
	data_ = data;

	basePos_ = owner_->GetPosition();

	rotaL_ = std::numbers::pi_v<float> / 2.0f;
	rotaR_ = -std::numbers::pi_v<float> / 2.0f;

	if (LR == "L")
	{
		rotationY_ = rotaL_;
	}
	if (LR == "R")
	{
		rotationY_ = rotaR_;
	}
}

void PlayerAttackComponent::Update(GameObject* owner)
{
	deltaTime_ = TimeManager::GetInstance().GetDeltaTime();

	ProcessAttack();
}

void PlayerAttackComponent::ProcessAttack()
{
	if (timer_ >= interval_)
	{
		//if (Input::GetInstance()->TriggerKey(DIK_RETURN))
		{
			timer_ = 0;
		}
	}
	else
	{
		timer_ += deltaTime_ * data_->info.stats.attackSpeed.final();
		owner_->SetPosition(basePos_);
		owner_->SetRotation({0,0,0});
		if (timer_ <= interval_ / 2.0f)
		{
			float t = timer_ / (interval_ / 2.0f);

			if (t < 0.5f)
			{
				t *= 2.0f;
			}
			else
			{
				t = 1.0f - (t - 0.5f) * 2.0f;
			}

			float rota = rotationY_ * t;
			Matrix4x4 rotaMat = MakeRotateYMatrix(rota);
			owner_->SetPosition(MathUtils::Transform(basePos_, rotaMat));
			owner_->SetRotation({ 0,rota,0 });
		}
	}
}
