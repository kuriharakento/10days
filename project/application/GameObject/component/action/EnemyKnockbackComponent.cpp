#include "EnemyKnockbackComponent.h"

#include "../../../../engine/math/Easing.h"

#include "application/GameObject/base/GameObject.h"
#include "application/GameObject/Combatable/character/base/Character.h"
#include "time/TimeManager.h"

EnemyKnockbackComponent::EnemyKnockbackComponent()
{
}

void EnemyKnockbackComponent::StartKnokback(const Vector3& start, const Vector3& target, float duration)
{
    startPos_ = start;
    targetPos_ = target;
    duration_ = duration;
    elapsed_ = 0.0f;
    isKnockback_ = true;
}

void EnemyKnockbackComponent::Update(GameObject* owner)
{
    if (!isKnockback_)
    {
        return;
    }

    elapsed_ += TimeManager::GetInstance().GetDeltaTime();
    float t = std::min(elapsed_ / duration_, 1.0f);
    //Vector3 newPos = startPos_ * (1.0f - t) + targetPos_ * t;
    Vector3 newPos = MathUtils::Lerp(startPos_, targetPos_, EaseOutQuad(t));
    owner->SetPosition(newPos);

    if (t >= 1.0f)
    {
        isKnockback_ = false;
    }
}

bool EnemyKnockbackComponent::IsKnockback() const
{
	return isKnockback_;
}
