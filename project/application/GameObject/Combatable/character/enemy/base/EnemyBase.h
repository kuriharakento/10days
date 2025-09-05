#pragma once

#include "../../base/Character.h"

class EnemyBase : virtual public Character
{
public:

	using OnDeathCallback = std::function<void(EnemyBase*)>;

	virtual ~EnemyBase() = default;
	EnemyBase() : Character(GameObjectTag::Common::EnemyBase) {}

	virtual void Initialize(Object3dCommon* object3dCommon, LightManager* lightManager, float* moveSpeed, GameObject* target);

	void Update() override;

	void Draw(CameraManager* camera) override;

	GameObject* GetTarget() const { return target_; }

	void SetOnDeathCallback(OnDeathCallback callback) { onDeathCallback_ = callback; }
	void CallOnDeath()
	{
		if (onDeathCallback_)
		{
			onDeathCallback_(this);
		}
	}

protected:

	float* moveSpeed_ = nullptr; // 移動速度
	GameObject* target_ = nullptr; // 攻撃対象

	OnDeathCallback onDeathCallback_ = nullptr; // 死亡時のコールバック

};

