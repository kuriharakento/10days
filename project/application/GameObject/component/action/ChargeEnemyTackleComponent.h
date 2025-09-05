#pragma once

#include "EnemyMoveComponent.h"
 
class ChargeEnemyTackleComponent : public EnemyMoveComponent
{
public:

	ChargeEnemyTackleComponent(float* moveSpeed, GameObject* target);

	void Update(GameObject* owner) override;

private:

	float chargeTimer_ = 0.0f; // 突進までのタイマー
	float tackleTimer_ = 0.0f; // 突進中のタイマー
	bool isCharging_ = false; // チャージアクション中かどうか
	bool isTackling_ = false; // 突進アクション中かどうか
	Vector3 tackleDirection_ = { 0.0f, 0.0f, 0.0f }; // 突進方向

};

