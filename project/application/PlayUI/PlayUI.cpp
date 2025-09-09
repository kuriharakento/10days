#include "PlayUI.h"
#include "manager/graphics/TextureManager.h"

void PlayUI::Initialize() 
{
	ResisterIcon("Resources/icon/ADIcon.png", { 300,200 });
	ResisterIcon("Resources/icon/ASIcon.png", { 590,200 });
	ResisterIcon("Resources/icon/MSIcon.png", { 880,200 });
	ResisterIcon("Resources/icon/KBIcon.png", { 450,500 });
	ResisterIcon("Resources/icon/HEALIcon.png", { 730,500 });

	ResisterUI("Resources/UI/hpxpFrame.png");
	ResisterUI("Resources/UI/LV.png", {0, 5});

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