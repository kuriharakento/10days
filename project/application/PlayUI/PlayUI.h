#pragma once

#include "engine/graphics/2d/Sprite.h"

#include "../GameObject/Combatable/character/player/Player.h"
#include "../../engine/graphics/2d/SpriteCommon.h"

class PlayUI
{
public:
	PlayUI(Player* player, SpriteCommon* sprcom) : player_(player), sprcom_(sprcom) {};

	void Initialize();
	void Update();
	void Draw();

private:
	void ResisterIcon(const std::string& path, Vector2 pos = { 0,0 });

	void ResisterUI(const std::string& path, Vector2 pos = { 0,0 }, Vector2 anc = { 0,0 });

private:
	Player* player_ = nullptr;

	SpriteCommon* sprcom_ = nullptr;

	std::vector<std::unique_ptr<Sprite>> upgradeIcons_;

	std::vector<std::unique_ptr<Sprite>> uiSpr_;
	std::unique_ptr<Sprite> hpbar_;
	std::unique_ptr<Sprite> xpbar_;
	Vector2 hpsize_ = {};
	Vector2 xpsize_ = {};
};

