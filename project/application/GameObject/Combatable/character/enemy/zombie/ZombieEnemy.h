#pragma once

#include "../base/EnemyBase.h"

class ZombieEnemy : public EnemyBase
{
public:

	ZombieEnemy() : Character(GameObjectTag::Character::ZombieEnemy) {}

	void Initialize(Object3dCommon* object3dCommon, LightManager* lightManager, float* moveSpeed, GameObject* target) override;
	
	void Update() override;
	
	void Draw(CameraManager* camera) override;

	void CollisionSettings(ICollisionComponent* collider)override;

private:

	bool isKnockback_ = false;
	Vector3 knockbackStartPos_;
	Vector3 knockbackTargetPos_;
	float knockbackElapsed_ = 0.0f;
	float knockbackDuration_ = 0.2f; // ノックバックにかける時間（秒）


};

