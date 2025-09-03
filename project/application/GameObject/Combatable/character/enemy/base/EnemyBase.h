#pragma once

#include "../../base/Character.h"

class EnemyBase : virtual public Character
{
public:

	virtual ~EnemyBase() = default;
	EnemyBase() : Character(GameObjectTag::Common::EnemyBase) {}

	virtual void Initialize(Object3dCommon* object3dCommon, LightManager* lightManager, float* moveSpeed, GameObject* target);

	void Update() override;

	void Draw(CameraManager* camera) override;

	GameObject* GetTarget() const { return target_; }

protected:

	float* moveSpeed_ = nullptr; // 移動速度
	GameObject* target_ = nullptr; // 攻撃対象

};

