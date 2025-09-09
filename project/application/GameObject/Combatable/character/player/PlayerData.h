#pragma once
// editor
#include "PlayerInfo.h"
#include "engine/jsonEditor/JsonEditableBase.h"

class PlayerData : public JsonEditableBase
{
public:
	PlayerData();
	void DrawImGui() override;
	void ResetStats();
	PlayerInfo info; // プレイヤー情報
};

