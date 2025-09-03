#pragma once
#include "application/GameObject/component/base/IGameObjectComponent.h"
#include "math/Vector3.h"
#include "input/Input.h"
#include "../../Combatable/character/player/PlayerData.h"

class GameObject;

class PlayerXPComponent : public IGameObjectComponent
{
public:
    PlayerXPComponent(PlayerData* data);
    void Update(GameObject* owner) override;

private:
    void ProcessAddXP();

private:
    PlayerData* data_ = nullptr;
};

