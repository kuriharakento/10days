#pragma once

#include "engine/graphics/2d/Sprite.h"
#include "engine/graphics/2d/SpriteCommon.h"

class TitleUI
{
public:
	TitleUI(SpriteCommon* sprcom) : sprcom_(sprcom) {};

	void Initialize();
	void Update();
	void Draw();

private:

	SpriteCommon* sprcom_ = nullptr;

	std::unique_ptr<Sprite> ui_;

};

