#pragma once
#include <memory>
#include <string>

#include "graphics/2d/Sprite.h"
#include "math/Vector4.h"
#include "math/VectorColorCodes.h"

enum class FadeType
{
	FadeIn,
	FadeOut,
};

class Fade
{
public:
	Fade();
	~Fade();

	// 初期化
	void Initialize(const std::string& texturePath, SpriteCommon* spriteCommon);

	// 描画
	void Draw();

	// 開始
	void Start(FadeType type, float duration, const Vector4& color = VectorColorCodes::Black);

	// 終了
	bool IsFinished() const;

	// リセット
	void Reset();

	// イージング関数の設定
	void SetEasingFunction(float (*func)(float));

private:
	float (*easingFunc_)(float);
	bool isActive_;        // フェードがアクティブかどうか
	std::unique_ptr<Sprite> sprite_; // フェード用のスプライト
};

