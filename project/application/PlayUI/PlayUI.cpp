#include "PlayUI.h"
#include "manager/graphics/TextureManager.h"
#include "input/Input.h"
#include "time/TimeManager.h"

#include <algorithm>
#include <cmath>

// 1) 線形（三角波）
inline float Triangle01(float t)
{
	t = std::clamp(t, 0.0f, 1.0f);
	return 1.0f - std::fabs(2.0f * t - 1.0f); // 0→1→0
}

// 2) なめらか（半サイン）
inline float ArchSine01(float t)
{
	t = std::clamp(t, 0.0f, 1.0f);
	constexpr float PI = 3.14159265358979323846f;
	return 0.5f - 0.5f * std::cos(2.0f * PI * t); // 0→1→0
}

Vector2 Lerp(const Vector2& start, const Vector2& end, float t)
{
	return start + (end - start) * t;
}

void PlayUI::Initialize() 
{
	ResisterIcon("Resources/icon/ADIcon.png", { 300,200 });
	ResisterIcon("Resources/icon/ASIcon.png", { 590,200 });
	ResisterIcon("Resources/icon/MSIcon.png", { 880,200 });
	ResisterIcon("Resources/icon/KBIcon.png", { 450,500 });
	ResisterIcon("Resources/icon/HEALIcon.png", { 730,500 });
	ResisterIcon("Resources/icon/lvupUI.png", { 730,350 });

	ResisterUI("Resources/UI/hpxpFrame.png");
	ResisterUI("Resources/UI/LV.png", { 0, 5 });
	ResisterUI("Resources/UI/operation.png");

	TextureManager::GetInstance()->LoadTexture("Resources/UI/hpbar.png");
	hpbar_ = std::make_unique<Sprite>();
	hpbar_->Initialize(sprcom_, "Resources/UI/hpbar.png");
	hpbar_->SetPosition({ 277,611 });
	hpsize_ = hpbar_->GetSize();

	TextureManager::GetInstance()->LoadTexture("Resources/UI/xpbar.png");
	xpbar_ = std::make_unique<Sprite>();
	xpbar_->Initialize(sprcom_, "Resources/UI/xpbar.png");
	xpbar_->SetPosition({ 277,645 });
	xpsize_ = xpbar_->GetSize();


	TextureManager::GetInstance()->LoadTexture("Resources/UI/number.png");
	for (int i = 0; i < 2; ++i)
	{
		lvSpr_[i] = std::make_unique<Sprite>();
		lvSpr_[i]->Initialize(sprcom_, "Resources/UI/number.png");
		lvSpr_[i]->SetPosition({ 172.0f + (float(i) * 40.0f),598.0f });
		lvSpr_[i]->SetSize({ 64,64 });
		lvSpr_[i]->SetTextureSize({ 64,64 });
	}
}

void PlayUI::Update()
{
	hpbar_->SetSize({ hpsize_.x * (player_->GetPlayerData()->info.stats.hitPoint / player_->GetPlayerData()->info.stats.maxHitPoint), hpsize_.y });
	hpbar_->Update();
	float p = (player_->GetPlayerData()->info.xp.xpIntoLevel / player_->GetPlayerData()->info.xp.xpToNextThreshold);
	xpbar_->SetSize({ xpsize_.x * p, xpsize_.y });
	xpbar_->Update();
	for (auto&& spr : upgradeIcons_)
	{
		spr->Update();
	}
	for (auto&& spr : uiSpr_)
	{
		spr->Update();
	}
	for (int i = 0; i < 2; ++i)
	{
		lvSpr_[i]->Update();
	}
	int ten = player_->GetPlayerData()->info.xp.level / 10;
	lvSpr_[0]->SetTextureLeftTop({ 64.0f * float(ten), 0 });
	int one = player_->GetPlayerData()->info.xp.level % 10;
	lvSpr_[1]->SetTextureLeftTop({ 64.0f * float(one), 0 });

	MOIcon();
}

void PlayUI::Draw()
{
	for (auto&& spr : uiSpr_)
	{
		spr->Draw();
	}
	hpbar_->Draw();
	xpbar_->Draw();
	for (int i = 0; i < 2; ++i)
	{
		lvSpr_[i]->Draw();
	}
	if (player_->GetIsUpgrade())
	{
		for (auto&& spr : upgradeIcons_)
		{
			spr->Draw();
		}
	}
}

void PlayUI::ResisterIcon(const std::string& path, Vector2 pos)
{
	TextureManager::GetInstance()->LoadTexture(path);
	auto sprite = std::make_unique<Sprite>();
	sprite->Initialize(sprcom_, path);
	sprite->SetPosition(pos);
	sprite->SetAnchorPoint({ 0.5f,0.5f });
	upgradeIcons_.push_back(std::move(sprite));
}

void PlayUI::ResisterUI(const std::string& path, Vector2 pos, Vector2 anc)
{
	TextureManager::GetInstance()->LoadTexture(path);
	auto sprite = std::make_unique<Sprite>();
	sprite->Initialize(sprcom_, path);
	sprite->SetPosition(pos);
	sprite->SetAnchorPoint(anc);
	uiSpr_.push_back(std::move(sprite));
}

void PlayUI::MOIcon()
{
	if (timer_ > easeTime_)
	{
		timer_ = 0;
	}

	timer_ += TimeManager::GetInstance().GetRealDeltaTime();
	float t = timer_ / easeTime_;

	Vector2 mousePos = { Input::GetInstance()->GetMouseX(), Input::GetInstance()->GetMouseY() };

	if (mousePos.x >= 200.0f && mousePos.x <= 400.0f &&
		mousePos.y >= 100.0f && mousePos.y <= 300.0f)
	{
		// AD
		upgradeIcons_[0]->SetSize(Lerp(base_, max_, Triangle01(t)));
	}
	else
	{
		upgradeIcons_[0]->SetSize(base_);
	}

	if (mousePos.x >= 490.0f && mousePos.x <= 690.0f &&
		mousePos.y >= 100.0f && mousePos.y <= 300.0f)
	{
		// AS
		upgradeIcons_[1]->SetSize(Lerp(base_, max_, Triangle01(t)));
	}
	else
	{
		upgradeIcons_[1]->SetSize(base_);
	}

	if (mousePos.x >= 780.0f && mousePos.x <= 980.0f &&
		mousePos.y >= 100.0f && mousePos.y <= 300.0f)
	{
		// MS
		upgradeIcons_[2]->SetSize(Lerp(base_, max_, Triangle01(t)));
	}
	else
	{
		upgradeIcons_[2]->SetSize(base_);
	}

	if (mousePos.x >= 350.0f && mousePos.x <= 550.0f &&
		mousePos.y >= 400.0f && mousePos.y <= 600.0f)
	{
		// KB
		upgradeIcons_[3]->SetSize(Lerp(base_, max_, Triangle01(t)));
	}
	else
	{
		upgradeIcons_[3]->SetSize(base_);
	}

	if (mousePos.x >= 630.0f && mousePos.x <= 830.0f &&
		mousePos.y >= 400.0f && mousePos.y <= 600.0f)
	{
		// HEAL
		upgradeIcons_[4]->SetSize(Lerp(base_, max_, Triangle01(t)));
	}
	else
	{
		upgradeIcons_[4]->SetSize(base_);
	}

	upgradeIcons_[5]->SetSize(Lerp(mouseUIBase_, mouseUIMax_, Triangle01(t)));

}
