#include "LevelUpComponent.h"

#include "time/TimeManager.h"

#include "../application/GameObject/Combatable/character/player/Player.h"

LevelUpComponent::LevelUpComponent(PlayerData* data)
{
	data_ = data;
}

void LevelUpComponent::Update(GameObject* owner)
{
	(void)owner;
	ProcessChoise(owner);
}

void LevelUpComponent::ProcessChoise(GameObject* owner)
{
	auto player = dynamic_cast<Player*>(owner);
	player->SetIsUpgrade(false);
	if (beforeLevel_ < data_->info.xp.level && data_->info.stats.hitPoint > 0.0f)
	{
		TimeManager::GetInstance().SetTimeScale(0.0f);
		player->SetIsUpgrade(true);
#ifdef _DEBUG
		ImGui::Begin("Choise");
		if (ImGui::Button("knockback"))
		{
			data_->info.stats.knockback.upgradeCount++;
			beforeLevel_++;
			TimeManager::GetInstance().SetTimeScale(1.0f);
		}
		if (ImGui::Button("attackSpeed"))
		{
			data_->info.stats.attackSpeed.upgradeCount++;
			beforeLevel_++;
			TimeManager::GetInstance().SetTimeScale(1.0f);
		}
		if (ImGui::Button("attackDamage"))
		{
			data_->info.stats.attackDamage.upgradeCount++;
			beforeLevel_++;
			TimeManager::GetInstance().SetTimeScale(1.0f);
		}
		if (ImGui::Button("moveSpeed"))
		{
			data_->info.stats.moveSpeed.upgradeCount++;
			beforeLevel_++;
			TimeManager::GetInstance().SetTimeScale(1.0f);
		}
		if (ImGui::Button("heal"))
		{
			data_->info.stats.hitPoint += data_->info.stats.heal;
			beforeLevel_++;
			TimeManager::GetInstance().SetTimeScale(1.0f);
		}
		Vector2 mousePos = { Input::GetInstance()->GetMouseX(), Input::GetInstance()->GetMouseY() };
		ImGui::DragFloat2("mouse", &mousePos.x);
		ImGui::End();
#endif // _DEBUG

		if(Input::GetInstance()->IsMouseButtonTriggered(0))
		{
			Vector2 mousePos = { Input::GetInstance()->GetMouseX(), Input::GetInstance()->GetMouseY() };
			if (mousePos.x >= 200.0f && mousePos.x <= 400.0f &&
				mousePos.y >= 100.0f && mousePos.y <= 300.0f)
			{
				data_->info.stats.attackDamage.upgradeCount++;
				beforeLevel_++;
				TimeManager::GetInstance().SetTimeScale(1.0f);
			}
			if (mousePos.x >= 490.0f && mousePos.x <= 690.0f &&
				mousePos.y >= 100.0f && mousePos.y <= 300.0f)
			{
				data_->info.stats.attackSpeed.upgradeCount++;
				beforeLevel_++;
				TimeManager::GetInstance().SetTimeScale(1.0f);
			}
			if (mousePos.x >= 780.0f && mousePos.x <= 980.0f &&
				mousePos.y >= 100.0f && mousePos.y <= 300.0f)
			{
				data_->info.stats.moveSpeed.upgradeCount++;
				beforeLevel_++;
				TimeManager::GetInstance().SetTimeScale(1.0f);
			}
			if (mousePos.x >= 350.0f && mousePos.x <= 550.0f &&
				mousePos.y >= 400.0f && mousePos.y <= 600.0f)
			{
				data_->info.stats.knockback.upgradeCount++;
				beforeLevel_++;
				TimeManager::GetInstance().SetTimeScale(1.0f);
			}
			if (mousePos.x >= 630.0f && mousePos.x <= 830.0f &&
				mousePos.y >= 400.0f && mousePos.y <= 600.0f)
			{
				data_->info.stats.hitPoint += data_->info.stats.heal;
				data_->info.stats.hitPoint = std::min<float>(data_->info.stats.hitPoint, data_->info.stats.maxHitPoint);
				beforeLevel_++;
				TimeManager::GetInstance().SetTimeScale(1.0f);
			}
		}
	}
}
