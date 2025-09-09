#include "ZoneShrinkComponent.h"

#include "application/GameObject/component/collision/SphereColliderComponent.h"
#include "effects/particle/component/group/MaterialColorComponent.h"
#include "effects/particle/component/group/UVTranslateComponent.h"
#include "effects/particle/component/single/ColorFadeOutComponent.h"
#include "effects/particle/component/single/ScaleOverLifetimeComponent.h"
#include "math/VectorColorCodes.h"
#include "time/TimeManager.h"

ZoneShrinkComponent::ZoneShrinkComponent(float shrinkSpeed)
{
	shrinkSpeed_ = shrinkSpeed;
}

void ZoneShrinkComponent::Update(GameObject* owner)
{
	if (isFirstFrame_)
	{
		// エミッターの初期化
		InitEmitter(owner);
		isFirstFrame_ = false;
	}

	if (owner->GetScale().x > shrinkMin_)
	{
		// ゾーンの縮小は XZ 平面のみ
		Vector3 newScale = owner->GetScale() - Vector3{ shrinkSpeed_ * TimeManager::GetInstance().GetDeltaTime(), 0.0f, shrinkSpeed_ * TimeManager::GetInstance().GetDeltaTime() };
		owner->SetScale(newScale);
		// ゾーンエフェクトの拡大
		if (zoneEffect_)
		{
			zoneEffect_->SetInitialScale(newScale);
		}

		// ゾーンの大きさに合わせて当たり判定を更新
		auto collider = owner->GetComponent<SphereColliderComponent>();
		collider->SetSphere(Sphere(owner->GetPosition(), owner->GetScale().x));
	}
}

void ZoneShrinkComponent::InitEmitter(GameObject* owner)
{
	zoneEffect_ = std::make_unique<ParticleEmitter>();
	zoneEffect_->Initialize("zoneEffect", "./Resources/gradationLine.png");
	zoneEffect_->SetEmitRange({}, {}); // 座標の位置で発生
	zoneEffect_->SetInitialLifeTime(0.04f);
	zoneEffect_->SetEmitRate(0.0f);
	zoneEffect_->SetBillborad(false);
	zoneEffect_->SetInitialScale(owner->GetScale());
	zoneEffect_->SetInitialRotation(Vector3{ std::numbers::pi_v<float>, 0.0f, 0.0f }); //　地面に倒す角度にする
	zoneEffect_->SetInitialColor(VectorColorCodes::Red);
	// 円柱を使用する
	zoneEffect_->SetModelType(ParticleGroup::ParticleType::Cylinder);
	// Y軸回転を加える
	zoneEffect_->AddComponent(std::make_shared<UVTranslateComponent>(Vector3{ 0.03f, 0.0f, 0.0f })); // UVを毎フレーム大きくずらす
	
	zoneEffect_->Start(
		&owner->GetPosition(),
		1,
		0.0f,
		true
	);
}
