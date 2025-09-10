#include "ResultUI.h"
#include "manager/graphics/TextureManager.h"
#include "Result.h"

void ResultUI::Initialize()
{
	ResisterUI("Resources/UI/result.png");

	auto ins = Result::GetInstance();


	TextureManager::GetInstance()->LoadTexture("Resources/UI/number.png");
	for (int i = 0; i < 3; ++i)
	{
		timeSpr_[i] = std::make_unique<Sprite>();
		timeSpr_[i]->Initialize(sprcom_, "Resources/UI/number.png");
		timeSpr_[i]->SetPosition({ 633.0f + (float(i) * 40.0f),220.0f });
		timeSpr_[i]->SetSize({ 64,64 });
		timeSpr_[i]->SetTextureSize({ 64,64 });
	}
	int hundred = int(ins->GetLifeTime()) / 100;
	timeSpr_[0]->SetTextureLeftTop({ 64.0f * float(hundred), 0 });
	int buf = int(ins->GetLifeTime()) % 100;
	int ten = buf / 10;
	timeSpr_[1]->SetTextureLeftTop({ 64.0f * float(ten), 0 });
	int one = buf % 10;
	timeSpr_[2]->SetTextureLeftTop({ 64.0f * float(one), 0 });

	for (int i = 0; i < 3; ++i)
	{
		enemySpr_[i] = std::make_unique<Sprite>();
		enemySpr_[i]->Initialize(sprcom_, "Resources/UI/number.png");
		enemySpr_[i]->SetPosition({ 633.0f + (float(i) * 40.0f),330.0f });
		enemySpr_[i]->SetSize({ 64,64 });
		enemySpr_[i]->SetTextureSize({ 64,64 });
	}
	hundred = ins->GetEnemyNum() / 100;
	enemySpr_[0]->SetTextureLeftTop({ 64.0f * float(hundred), 0 });
	buf = ins->GetEnemyNum() % 100;
	ten = buf / 10;
	enemySpr_[1]->SetTextureLeftTop({ 64.0f * float(ten), 0 });
	one = buf % 10;
	enemySpr_[2]->SetTextureLeftTop({ 64.0f * float(one), 0 });

	for (int i = 0; i < 2; ++i)
	{
		lvSpr_[i] = std::make_unique<Sprite>();
		lvSpr_[i]->Initialize(sprcom_, "Resources/UI/number.png");
		lvSpr_[i]->SetPosition({ 633.0f + (float(i) * 40.0f),448.0f });
		lvSpr_[i]->SetSize({ 64,64 });
		lvSpr_[i]->SetTextureSize({ 64,64 });
	}
	ten = ins->GetLevel() / 10;
	lvSpr_[0]->SetTextureLeftTop({ 64.0f * float(ten), 0 });
	one = ins->GetLevel() % 10;
	lvSpr_[1]->SetTextureLeftTop({ 64.0f * float(one), 0 });
}

void ResultUI::Update()
{
	for (auto&& spr : uiSpr_)
	{
		spr->Update();
	}
	for (int i = 0; i < 3; ++i)
	{
		timeSpr_[i]->Update();
	}
	for (int i = 0; i < 3; ++i)
	{
		enemySpr_[i]->Update();
	}
	for (int i = 0; i < 2; ++i)
	{
		lvSpr_[i]->Update();
	}
}

void ResultUI::Draw()
{
	for (auto&& spr : uiSpr_)
	{
		spr->Draw();
	}
	for (int i = 0; i < 3; ++i)
	{
		timeSpr_[i]->Draw();
	}
	for (int i = 0; i < 3; ++i)
	{
		enemySpr_[i]->Draw();
	}
	for (int i = 0; i < 2; ++i)
	{
		lvSpr_[i]->Draw();
	}
}

void ResultUI::ResisterUI(const std::string& path, Vector2 pos, Vector2 anc)
{
	TextureManager::GetInstance()->LoadTexture(path);
	auto sprite = std::make_unique<Sprite>();
	sprite->Initialize(sprcom_, path);
	sprite->SetPosition(pos);
	sprite->SetAnchorPoint(anc);
	uiSpr_.push_back(std::move(sprite));
}
