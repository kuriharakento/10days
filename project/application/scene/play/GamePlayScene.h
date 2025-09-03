#pragma once
// scene
#include "engine/scene/interface/BaseScene.h"

#include "application/GameObject/Combatable/character/player/Player.h"

class GamePlayScene : public BaseScene
{
public:
	//初期化
	void Initialize() override;
	//終了
	void Finalize() override;
	//更新
	void Update() override;
	//描画
	void Draw3D() override;
	void Draw2D() override;

private:
	std::unique_ptr<Player> player_;
};

