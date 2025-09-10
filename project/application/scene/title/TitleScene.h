#pragma once
#include <memory>

// app

// camerawork
#include "camerawork/debug/DebugCamera.h"
#include "camerawork/spline/SplineCamera.h"
#include "camerawork/topDown/TopDownCamera.h"

// scene
#include "engine/scene/interface/BaseScene.h"

// graphics
#include "graphics/3d/Object3d.h"

// effects
#include "application/effect/Fade.h"
#include "application/GameObject/base/GameObject.h"
#include "application/GameObject/Combatable/character/player/Player.h"
#include "application/GameObject/zone/Zone.h"
#include "camerawork/orbit/OrbitCameraWork.h"
#include "effects/particle/ParticleEmitter.h"

class TitleScene : public BaseScene
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
	// ImGuiの描画
	void DrawImGui();

private: //メンバ変数
	// カメラワーク
	std::unique_ptr<DebugCamera> debugCamera_;
	// オービットカメラ
	std::unique_ptr<OrbitCameraWork> orbitCamera_;
	// プレイヤー
	std::unique_ptr<Player> player_;
	// スカイドーム
	std::unique_ptr<GameObject> skydome_;
	// パーティクル
	std::unique_ptr<ParticleEmitter> playerParticle_;
	std::unique_ptr<ParticleEmitter> zoneEffect_;
	// フェード
	std::unique_ptr<Fade> fade_;
	bool nextScene_ = false;
};
