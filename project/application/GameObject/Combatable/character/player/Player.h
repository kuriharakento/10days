#pragma once
#include "application/GameObject/Combatable/character/base/Character.h"
#include "application/GameObject/component/base/ICollisionComponent.h"

#include "PlayerData.h"

class Player : public Character
{
public:
	~Player();
	Player(std::string tag = GameObjectTag::Character::Player) : Character(tag) {}
	void Initialize(Object3dCommon* object3dCommon, LightManager* lightManager);
	void Update() override;
	void Draw(CameraManager* camera) override;

	void SetIsUpgrade(bool is) { isUpgrade_ = is; }
	bool GetIsUpgrade() { return isUpgrade_; }

	void XPGain(float xpGain){ data_->info.xp.addXP(xpGain); }

private:
	void CollisionSettings(ICollisionComponent* collider) override;

private:
	std::shared_ptr<PlayerData> data_;

	bool isUpgrade_ = false;
};

