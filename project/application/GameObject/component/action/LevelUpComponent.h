#pragma once
#include "application/GameObject/component/base/IGameObjectComponent.h"
#include "math/Vector3.h"
#include "input/Input.h"
#include "../../Combatable/character/player/PlayerData.h"

class GameObject;

class LevelUpComponent : public IGameObjectComponent
{
public:
	LevelUpComponent(PlayerData* data);
	void Update(GameObject* owner) override;

private:
	void ProcessChoise(GameObject* owner);

private:
	PlayerData* data_ = nullptr;
	int beforeLevel_ = 1;
	bool levelUp_ = false;
};

