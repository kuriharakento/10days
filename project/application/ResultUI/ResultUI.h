#pragma once
#include "engine/graphics/2d/Sprite.h"

#include "../../engine/graphics/2d/SpriteCommon.h"

class ResultUI
{
public:
	ResultUI(SpriteCommon* sprcom) : sprcom_(sprcom) {};

	void Initialize();
	void Update();
	void Draw();

private:
	void ResisterUI(const std::string& path, Vector2 pos = { 0,0 }, Vector2 anc = { 0,0 });

private:
	SpriteCommon* sprcom_ = nullptr;

	std::vector<std::unique_ptr<Sprite>> uiSpr_;

	std::array<std::unique_ptr<Sprite>, 3> timeSpr_;

	std::array<std::unique_ptr<Sprite>, 3> enemySpr_;

	std::array<std::unique_ptr<Sprite>, 2> lvSpr_;
};

