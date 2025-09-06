#pragma once
#include "application/GameObject/component/base/IGameObjectComponent.h"
#include "math/Vector3.h"
#include "input/Input.h"
#include "../../Combatable/character/player/PlayerData.h"

class GameObject;

class MoveComponent : public IGameObjectComponent
{
public:
    MoveComponent(PlayerData* data);
    void Update(GameObject* owner) override;

    // 移動パラメータ設定
    void SetMoveSpeed(float speed) { data_->info.stats.moveSpeed.baseValue = speed; }
    void SetRotationSpeed(float speed) { rotationSpeed_ = speed; }

private:
    void ProcessMovement(GameObject* owner);
    Vector3 GetMovementDirection() const;
    void UpdateRotation(GameObject* owner, const Vector3& direction);  // 向き補間処理

private:
    PlayerData* data_ = nullptr;

    // 回転補間
    float rotationSpeed_ = 0.1f;  // 回転補間速度

    bool hasMovementInput_ = false;         // 移動入力があるか

    float deltaTime = 1.0f / 60.0f;

};

