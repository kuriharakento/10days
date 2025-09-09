#include "PlayerData.h"

PlayerData::PlayerData()
{
	Register("playerInfo", &info);
}

void PlayerData::DrawImGui()
{
#ifdef _DEBUG
	ImGui::PushID("player");
	if (ImGui::TreeNode("CurrentStats"))
	{
		ImGui::PushID("CurrentStats");
		ImGui::DragFloat("HP", &info.stats.hitPoint);
		float knockback = info.stats.knockback.final();
		ImGui::DragFloat("KnockBack", &knockback);
		ImGui::PushItemWidth(ImGui::CalcItemWidth() * 0.5f);
		float attackSpeed = info.stats.attackSpeed.final();
		ImGui::DragFloat("AS", &attackSpeed);
		ImGui::SameLine();
		float attackDamage = info.stats.attackDamage.final();
		ImGui::DragFloat("AD", &attackDamage);
		float moveSpeed = info.stats.moveSpeed.final();
		ImGui::DragFloat("MS", &moveSpeed);
		ImGui::PopItemWidth();
		ImGui::PopID();
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("BaseStats"))
	{
		ImGui::PushID("BaseStats");
		ImGui::DragFloat("HP", &info.stats.maxHitPoint);
		ImGui::DragFloat("KnockBack", &info.stats.knockback.baseValue, 0.01f);
		ImGui::PushItemWidth(ImGui::CalcItemWidth() * 0.5f);
		ImGui::DragFloat("AS", &info.stats.attackSpeed.baseValue, 0.01f);
		ImGui::SameLine();
		ImGui::DragFloat("AD", &info.stats.attackDamage.baseValue, 0.01f);
		ImGui::DragFloat("MS", &info.stats.moveSpeed.baseValue, 0.01f);
		ImGui::PopItemWidth();
		ImGui::PopID();
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("UpgradeCount"))
	{
		ImGui::PushID("UpgradeCount");
		ImGui::DragInt("KnockBack", &info.stats.knockback.upgradeCount);
		ImGui::PushItemWidth(ImGui::CalcItemWidth() * 0.5f);
		ImGui::DragInt("AS", &info.stats.attackSpeed.upgradeCount);
		ImGui::SameLine();
		ImGui::DragInt("AD", &info.stats.attackDamage.upgradeCount);
		ImGui::DragInt("MS", &info.stats.moveSpeed.upgradeCount);
		ImGui::PopItemWidth();
		ImGui::PopID();
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("AddBonus(x100%)"))
	{
		ImGui::PushID("AddBonus");
		ImGui::DragFloat("KnockBack", &info.stats.knockback.increment, 0.01f);
		ImGui::PushItemWidth(ImGui::CalcItemWidth() * 0.5f);
		ImGui::DragFloat("AS", &info.stats.attackSpeed.increment, 0.01f);
		ImGui::SameLine();
		ImGui::DragFloat("AD", &info.stats.attackDamage.increment, 0.01f);
		ImGui::DragFloat("MS", &info.stats.moveSpeed.increment, 0.01f);
		ImGui::SameLine();
		ImGui::DragFloat("Heal", &info.stats.heal);
		ImGui::PopItemWidth();
		ImGui::PopID();
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("XP"))
	{
		ImGui::PushID("XP");
		ImGui::DragInt("Lv.", &info.xp.level);
		ImGui::DragFloat("totalXP", &info.xp.totalXP);
		ImGui::DragFloat("xpIntoLevel", &info.xp.xpIntoLevel);
		ImGui::DragFloat("xpToNextThreshold", &info.xp.xpToNextThreshold);
		float p = (info.xp.xpIntoLevel / info.xp.xpToNextThreshold) * 100.0f;
		ImGui::Text("NextLv.%.0f%%", p);
		ImGui::PopID();
		ImGui::TreePop();
	}
	if (ImGui::Button("ResetStats(XP&UpgradeCount)"))
	{
		ResetStats();
	}
	ImGui::PopID();
	ImGui::Separator(); 
#endif
}

void PlayerData::ResetStats()
{
	info.stats.hitPoint = 100.0f;
	info.stats.knockback.upgradeCount = 0;
	info.stats.attackSpeed.upgradeCount = 0;
	info.stats.attackDamage.upgradeCount = 0;
	info.stats.moveSpeed.upgradeCount = 0;
	info.xp.level = 0;
	info.xp.totalXP = 0;
	info.xp.xpIntoLevel = 0;
	info.xp.xpToNextThreshold = 0;
}
