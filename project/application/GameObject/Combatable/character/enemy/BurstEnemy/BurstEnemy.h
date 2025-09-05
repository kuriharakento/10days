#pragma once

#include "../base/EnemyBase.h"

class BurstEnemy : public EnemyBase
{
public:

	BurstEnemy() : Character(GameObjectTag::Character::BurstEnemy) {}
	
	void Initialize(Object3dCommon* object3dCommon, LightManager* lightManager, float* moveSpeed, GameObject* target) override;

	void Update() override;

	void Draw(CameraManager* camera) override;
	
	void CollisionSettings(ICollisionComponent* collider)override;

};

