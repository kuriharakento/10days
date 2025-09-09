#include "Fade.h"

#include "math/Easing.h"
#include "time/Timer.h"
#include "time/TimerManager.h"

Fade::Fade()
{
}

Fade::~Fade()
{
}

void Fade::Initialize(const std::string& texturePath, SpriteCommon* spriteCommon)
{
	// スプライトの初期化
	sprite_ = std::make_unique<Sprite>();
	sprite_->Initialize(spriteCommon, texturePath);
	sprite_->SetPosition({ 0.0f, 0.0f });
	sprite_->SetSize({ 1280.0f, 720.0f });				  // 画面全体を覆うサイズに設定
	sprite_->SetColor({ 0.0f, 0.0f, 0.0f, 0.0f }); // 初期は透明
	sprite_->SetAnchorPoint({ 0.0f, 0.0f }); // 左上を基準点に設定
	isActive_ = false; // 最初は非アクティブ
	easingFunc_ = EaseInSine<float>; // デフォルトのイージング関数
}

void Fade::Draw()
{
	if (isActive_)
	{
		sprite_->Update();
		sprite_->Draw(); // フェード中のみ描画
	}
}

void Fade::Start(FadeType type, float duration, const Vector4& color)
{
	// タイマー作成
	auto timer = std::make_unique<Timer>("Fade", duration, DeltaTimeType::RealDeltaTime);

	// 開始時のコールバック
	timer->SetOnStart(
		[this]() {
			isActive_ = true; // フェード中はアクティブにする
		});

	// 更新中のコールバック
	switch (type)
	{
	case FadeType::FadeIn:

		timer->SetOnTick(
			[this, duration, color](float t) {
				// 進行度
				float progress = t / duration;

				// ここでイージング関数適用！（例：EaseInSine）
				float alpha = easingFunc_(progress);

				// フェードインは透明→指定色
				sprite_->SetColor({ color.x, color.y, color.z, alpha });
			});
		break;
	case FadeType::FadeOut:
		timer->SetOnTick(
			[this, duration, color](float t) {
				// フェードアウトは指定色から透明へ
				// 進行度
				float progress = t / duration;

				// ここでイージング関数適用！（例：EaseInSine）
				float alpha = 1.0f - easingFunc_(progress);

				sprite_->SetColor({ color.x, color.y, color.z, alpha });
			});
		break;
	}

	// 終了時のコールバック
	timer->SetOnFinish(
		[this]() {
			isActive_ = false; // フェード終了後は非アクティブにする
		});

	// タイマーを登録
	TimerManager::GetInstance().AddTimer(std::move(timer));
}

bool Fade::IsFinished() const
{
	return !TimerManager::GetInstance().GetTimer("Fade") && isActive_;
}

void Fade::Reset()
{
	isActive_ = false;
}

void Fade::SetEasingFunction(float(* func)(float))
{
	easingFunc_ = func;
}
