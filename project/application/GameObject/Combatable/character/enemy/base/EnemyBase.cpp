#include "EnemyBase.h"

#include "../../../../component/action/EnemyMoveComponent.h"

void EnemyBase::Initialize(Object3dCommon* object3dCommon, LightManager* lightManager, float* moveSpeed, GameObject* target)
{
	Character::Initialize(object3dCommon, lightManager);

	// 移動速度を設定
	moveSpeed_ = moveSpeed;

	// ターゲットを設定
	target_ = target;

	AddComponent("EnemyMove", std::make_unique<EnemyMoveComponent>(moveSpeed_, target_));

}

void EnemyBase::Update()
{
	Character::Update();

	// 死亡処理
	if (hp_.base <= 0.0f)
	{
		isAlive_ = false;
	}

}

void EnemyBase::Draw(CameraManager* camera)
{
	Character::Draw(camera);
}
