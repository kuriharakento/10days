#include "PlayerXPComponent.h"

PlayerXPComponent::PlayerXPComponent(PlayerData* data)
{
	data_ = data;
}

void PlayerXPComponent::Update(GameObject* owner)
{
	(void)owner;
	ProcessAddXP();
}

void PlayerXPComponent::ProcessAddXP()
{
	if (Input::GetInstance()->TriggerKey(DIK_X))
	{
		float xpGain = 2.0f;
		int ups = data_->info.xp.addXP(xpGain);
	}
}
