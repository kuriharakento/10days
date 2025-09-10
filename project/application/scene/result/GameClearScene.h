#pragma once
#include "scene/interface/BaseScene.h"
// app
#include "application/effect/Fade.h"
#include "../../ResultUI/ResultUI.h"

class GameClearScene : public BaseScene
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
	std::unique_ptr<ResultUI> resultUI_;

	// フェード
	std::unique_ptr<Fade> fade_;
	bool nextScene_ = false;
};

