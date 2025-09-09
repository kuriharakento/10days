#pragma once

#include "application/GameObject/component/base/IActionComponent.h"
#include "math/Vector3.h"

class EnemyKnockbackComponent : public IActionComponent
{
public:

	EnemyKnockbackComponent();

	void StartKnokback(const Vector3& start, const Vector3& target, float duration);

	void Update(GameObject* owner) override;

	bool IsKnockback() const;

private:

	bool isKnockback_ = false;
	Vector3 startPos_;
	Vector3 targetPos_;
	float elapsed_ = 0.0f;
	float duration_ = 0.5f; // ノックバックにかける時間（秒）


};

