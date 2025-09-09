#include "Fade.h"

#include "math/Easing.h"
#include "time/TimeManager.h"
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
	duration_ = 0.0f;
	elapsedTime_ = 0.0f;
	easingFunc_ = EaseInSine<float>; // デフォルトのイージング関数
}

void Fade::Update()
{
	if (isActive_)
	{
		elapsedTime_ += TimeManager::GetInstance().GetDeltaTime();
		float t = elapsedTime_ / duration_;
		float alpha;
		switch (type_)
		{
		case FadeType::FadeIn:
			alpha = 1.0f - easingFunc_(t); // イージング関数を使用
			break;
		case FadeType::FadeOut:
			alpha = easingFunc_(t); // イージング関数を使用
		}
		// spriteのアルファ値を更新
		sprite_->SetColor({ sprite_->GetColor().x, sprite_->GetColor().y, sprite_->GetColor().z, alpha });
		sprite_->Update();

		if (elapsedTime_ >= duration_)
		{
			// フェード完了
			isActive_ = false;
			elapsedTime_ = 0.0f;
			duration_ = 0.0f;

			if (type_ == FadeType::FadeIn)
			{
				// フェードイン完了後は描画しない
				isDraw_ = false;
			}
			else if (type_ == FadeType::FadeOut)
			{
				// フェードアウト完了後は描画する
				isDraw_ = true;
			}
		}
	}
}

void Fade::Draw()
{
	if (isDraw_)
	{
		sprite_->Draw();
	}
}

void Fade::Start(FadeType type, float duration, const Vector4& color)
{
	type_ = type;
	duration_ = duration;
	sprite_->SetColor(color);
	isActive_ = true;
	isDraw_ = true;
}

bool Fade::IsActive() const
{
	return isActive_;
}

void Fade::Reset()
{
	isActive_ = false;
}

void Fade::SetEasingFunction(float(*func)(float))
{
	easingFunc_ = func;
}
