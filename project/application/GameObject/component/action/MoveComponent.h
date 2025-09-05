#pragma once
#include "application/GameObject/component/base/IGameObjectComponent.h"
#include "math/Vector3.h"
#include "input/Input.h"

class GameObject;

class MoveComponent : public IGameObjectComponent
{
public:
    MoveComponent();
    void Update(GameObject* owner) override;

    // 移動パラメータ設定
    void SetMoveSpeed(float speed) { moveSpeed_ = speed; }
    void SetRotationSpeed(float speed) { rotationSpeed_ = speed; }

private:
    void ProcessMovement(GameObject* owner);
    Vector3 GetMovementDirection() const;
    void UpdateRotation(GameObject* owner, const Vector3& direction);  // 向き補間処理

private:
    // 基本移動
    float moveSpeed_ = 9.0f;

    // 回転補間
    float rotationSpeed_ = 0.1f;  // 回転補間速度

    bool hasMovementInput_ = false;         // 移動入力があるか

    float deltaTime = 1.0f / 60.0f;

};

