#include "Zone.h"

#include "application/GameObject/component/action/ZoneShrinkComponent.h"
#include "application/GameObject/component/collision/SphereColliderComponent.h"
#include "math/VectorColorCodes.h"
#include "manager/effect/PostProcessManager.h"

void Zone::Initialize(Object3dCommon* object3dCommon, LightManager* lightManager, Camera* camera, PostProcessManager* postProcess)
{
	GameObject::Initialize(object3dCommon, lightManager, camera);

	// 初期位置、スケール設定
	SetPosition({ 0.0f, 10.0f, 0.0f });
	SetScale({ 20.0f, 10.0f, 20.0f });

	// コンポーネント追加
	// 当たり判定コンポーネント
	auto zoneCollider = std::make_unique<SphereColliderComponent>(this);
	zoneCollider->SetOnEnter([this, postProcess](GameObject* other) {
		if (other->GetTag() == GameObjectTag::Character::Player)
		{
			postProcess->grayscaleEffect_->SetEnabled(false);
		}
							 });
	zoneCollider->SetOnStay([](GameObject* other) {

							});
	zoneCollider->SetOnExit([this, postProcess](GameObject* other) {
		if (other->GetTag() == GameObjectTag::Character::Player)
		{
			postProcess->grayscaleEffect_->SetEnabled(true);
			postProcess->grayscaleEffect_->SetIntensity(1.0f);
		}
							});
	AddComponent("SphereCollider", std::move(zoneCollider));

	// ゾーン縮小コンポーネント
	AddComponent("ZoneShrink", std::make_unique<ZoneShrinkComponent>());
}

void Zone::Update()
{
	GameObject::Update();
}

void Zone::Draw(CameraManager* camera)
{
	// ゾーンはパーティクルで表現するので描画せず、行列の更新のみ行う
	UpdateTransform(camera);
}
