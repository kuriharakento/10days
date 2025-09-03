#include "LevelUpComponent.h"

LevelUpComponent::LevelUpComponent(PlayerData* data)
{
	data_ = data;
}

void LevelUpComponent::Update(GameObject* owner)
{
	(void)owner;
	ProcessChoise();
}

void LevelUpComponent::ProcessChoise()
{
	if (beforeLevel_ != data_->info.xpState.level)
	{
#ifdef _DEBUG
		ImGui::Begin("Choise");
		if (ImGui::Button("knockback"))
		{
			data_->info.multiplier.knockback += data_->info.addBonus.knockback;
			beforeLevel_++;
		}
		if (ImGui::Button("attackSpeed"))
		{
			data_->info.multiplier.attackSpeed += data_->info.addBonus.attackSpeed;
			beforeLevel_++;
		}
		if (ImGui::Button("attackDamage"))
		{
			data_->info.multiplier.attackDamage += data_->info.addBonus.attackDamage;
			beforeLevel_++;
		}
		if (ImGui::Button("moveSpeed"))
		{
			data_->info.multiplier.moveSpeed += data_->info.addBonus.moveSpeed;
			beforeLevel_++;
		}
		if (ImGui::Button("heal"))
		{
			data_->info.status.hitPoint += data_->info.addBonus.heal;
			beforeLevel_++;
		}
		ImGui::End();
#endif // _DEBUG

	}
}
