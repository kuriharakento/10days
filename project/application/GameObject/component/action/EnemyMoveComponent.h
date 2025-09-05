#pragma once

#include "application/GameObject/component/base/IActionComponent.h"
#include "math/Vector3.h"

class EnemyMoveComponent : public IActionComponent
{
public:

	explicit EnemyMoveComponent(float* speed,GameObject* target);
	
	void Update(GameObject* owner) override;
	
	void Draw(CameraManager* camera) override {}

private:

	float speed_;            // 移動速度
	GameObject* target_;     // 移動目標

};

