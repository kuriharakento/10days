#pragma once
#include "application/GameObject/component/base/IActionComponent.h"
#include "effects/particle/ParticleEmitter.h"

class ZoneShrinkComponent : public IActionComponent
{
public:
    /// @param 
    explicit ZoneShrinkComponent(float shrinkSpeed = 0.8f);

    /// フレームごとに垂直速度を更新し、Y=0 より下には落とさない
    void Update(GameObject* owner) override;

    /// 描画は不要
    void Draw(CameraManager* camera) override {}

private:
    void InitEmitter(GameObject* owner);

private:
	// 最初のフレームに一度だけ実行
	bool isFirstFrame_ = true;

	// 縮小速度
	float shrinkSpeed_;

    // 最小縮小
	float shrinkMin_ = 0.01f;

    // ゾーンをパーティクルで表現
	std::unique_ptr<ParticleEmitter> zoneEffect_;
};
