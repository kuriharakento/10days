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
		int ups = AddXP(data_->info.xpState, data_->info.xpParams, xpGain);
	}
}
