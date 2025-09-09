#pragma once
#include "application/GameObject/base/GameObject.h"
#include "effects/particle/ParticleEmitter.h"

class PostProcessManager;

class Zone : public GameObject
{
public:
	explicit Zone(const std::string& tag = GameObjectTag::Field::Zone) : GameObject(tag) {}
	void Initialize(Object3dCommon* object3dCommon, LightManager* lightManager, Camera* camera, PostProcessManager* postProcess);
	void Update() override;
	void Draw(CameraManager* camera) override;

private:
	// 範囲外にいるときのエフェクト
	std::unique_ptr<ParticleEmitter> outEffect_;
};

