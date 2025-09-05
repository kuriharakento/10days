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
	if (beforeLevel_ != data_->info.xp.level)
	{
#ifdef _DEBUG
		ImGui::Begin("Choise");
		if (ImGui::Button("knockback"))
		{
			data_->info.stats.knockback.upgradeCount++;
			beforeLevel_++;
		}
		if (ImGui::Button("attackSpeed"))
		{
			data_->info.stats.attackSpeed.upgradeCount++;
			beforeLevel_++;
		}
		if (ImGui::Button("attackDamage"))
		{
			data_->info.stats.attackDamage.upgradeCount++;
			beforeLevel_++;
		}
		if (ImGui::Button("moveSpeed"))
		{
			data_->info.stats.moveSpeed.upgradeCount++;
			beforeLevel_++;
		}
		if (ImGui::Button("heal"))
		{
			data_->info.stats.hitPoint += data_->info.stats.heal;
			beforeLevel_++;
		}
		ImGui::End();
#endif // _DEBUG
	}
}
