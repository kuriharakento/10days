#include "PlayerData.h"

PlayerData::PlayerData()
{
	Register("playerInfo", &info);
}

void PlayerData::DrawImGui()
{
#ifdef _DEBUG
	ImGui::PushID("player");
	if (ImGui::TreeNode("DefaultStatus"))
	{
		ImGui::DragFloat("HP", &info.status.hitPoint);
		ImGui::DragFloat("KnockBack", &info.status.knockback);
		ImGui::PushItemWidth(ImGui::CalcItemWidth() * 0.5f);
		ImGui::DragFloat("AS", &info.status.attackSpeed);
		ImGui::SameLine();
		ImGui::DragFloat("AD", &info.status.attackDamage);
		ImGui::DragFloat("MS", &info.status.moveSpeed);
		ImGui::SameLine();
		ImGui::DragFloat("Exp", &info.status.exp);
		ImGui::PopItemWidth();
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("StatusMultiplier"))
	{
		ImGui::DragFloat("KnockBack", &info.multiplier.knockback);
		ImGui::PushItemWidth(ImGui::CalcItemWidth() * 0.5f);
		ImGui::DragFloat("AS", &info.multiplier.attackSpeed);
		ImGui::SameLine();
		ImGui::DragFloat("AD", &info.multiplier.attackDamage);
		ImGui::DragFloat("MS", &info.multiplier.moveSpeed);
		ImGui::PopItemWidth();
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("AddBonus"))
	{
		ImGui::DragFloat("KnockBack", &info.addBonus.knockback);
		ImGui::PushItemWidth(ImGui::CalcItemWidth() * 0.5f);
		ImGui::DragFloat("AS", &info.addBonus.attackSpeed);
		ImGui::SameLine();
		ImGui::DragFloat("AD", &info.addBonus.attackDamage);
		ImGui::DragFloat("MS", &info.addBonus.moveSpeed);
		ImGui::SameLine();
		ImGui::DragFloat("Heal", &info.addBonus.heal);
		ImGui::PopItemWidth();
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("XP"))
	{
		ImGui::DragInt("Lv.", &info.xpState.level);
		ImGui::DragFloat("totalXP", &info.xpState.totalXP);
		ImGui::DragFloat("xpIntoLevel", &info.xpState.xpIntoLevel);
		ImGui::DragFloat("xpToNextThreshold", &info.xpState.xpToNextThreshold);
		float p = (info.xpState.xpIntoLevel / info.xpState.xpToNextThreshold) * 100.0f;
		ImGui::Text("NextLv.%.0f%%", p);
		ImGui::TreePop();
	}
	ImGui::PopID();
	ImGui::Separator(); 
#endif
}
