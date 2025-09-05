#pragma once

#include "../base/EnemyBase.h"

class RushEnemy : public EnemyBase
{
public:

	RushEnemy() : Character(GameObjectTag::Character::RushEnemy) {}
	
	void Initialize(Object3dCommon* object3dCommon, LightManager* lightManager, float* moveSpeed, GameObject* target) override;
	
	void Update() override;
	
	void Draw(CameraManager* camera) override;
	
	void CollisionSettings(ICollisionComponent* collider)override;
};

