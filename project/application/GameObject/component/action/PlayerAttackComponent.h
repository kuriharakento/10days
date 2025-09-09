#pragma once
#include "application/GameObject/component/base/IGameObjectComponent.h"
#include "math/Vector3.h"
#include "input/Input.h"
#include "../../Combatable/character/player/PlayerData.h"

class GameObject;

class PlayerAttackComponent : public IGameObjectComponent
{
public:
    PlayerAttackComponent(GameObject* owner, PlayerData* data, const char* LR);
    void Update(GameObject* owner) override;

private:
    void ProcessAttack(GameObject* owner);

private:
    GameObject* owner_ = nullptr;

    PlayerData* data_ = nullptr;

    Vector3 basePos_{};

    float rotationY_ = 0.0f;
    float rotaL_ = 0.0f;
    float rotaR_ = 0.0f;

    float deltaTime_ = 1.0f / 60.0f;

    float timer_ = 0.0f;
    float interval_ = 1.0f;
};

