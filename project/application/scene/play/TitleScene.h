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
#include "application/GameObject/base/GameObject.h"
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
	// パーティクルエミッターの初期化
	void InitializeParticleEmitters();
	// ImGuiの描画
	void DrawImGui();

private: //メンバ変数
	// カメラワーク
	std::unique_ptr<DebugCamera> debugCamera_;
	// エミッター
	std::unique_ptr<ParticleEmitter> dust_;
	std::unique_ptr<ParticleEmitter> redEffect_;
	std::unique_ptr<ParticleEmitter> fallHeart_;
	std::unique_ptr<ParticleEmitter> glitch_;
	std::unique_ptr<ParticleEmitter> mordeVFXGround_;
	std::unique_ptr<ParticleEmitter> mordeVFXFragment_;
	// デバッグ
	std::unique_ptr<GameObject> debugCube1_;
	std::unique_ptr<GameObject> debugCube2_;
};
