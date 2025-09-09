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

	TextureManager::GetInstance()->LoadTexture("Resources/UI/hpbar.png");
	hpbar_ = std::make_unique<Sprite>();
	hpbar_->Initialize(sprcom_, "Resources/UI/hpbar.png");
	hpbar_->SetPosition({ 277,611 });
	hpbar_->SetAnchorPoint({ 0,0 });
	hpsize_ = hpbar_->GetSize();

	TextureManager::GetInstance()->LoadTexture("Resources/UI/xpbar.png");
	xpbar_ = std::make_unique<Sprite>();
	xpbar_->Initialize(sprcom_, "Resources/UI/xpbar.png");
	xpbar_->SetPosition({ 277,645 });
	xpbar_->SetAnchorPoint({ 0,0 });
	xpsize_ = xpbar_->GetSize();
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
}

void PlayUI::Draw()
{
	for (auto&& spr : uiSpr_)
	{
		spr->Draw();
	}
	hpbar_->Draw();
	xpbar_->Draw();
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