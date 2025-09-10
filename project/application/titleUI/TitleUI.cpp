#include "TitleUI.h"

void TitleUI::Initialize()
{
	ui_ = std::make_unique<Sprite>();
	ui_->Initialize(sprcom_, "Resources/UI/titleUI.png");
	ui_->SetPosition({ 640.0f,360.0f });
	ui_->SetSize({ 1280.0f,720.0f });
}

void TitleUI::Update()
{
	ui_->Update();
}

void TitleUI::Draw()
{
	ui_->Draw();
}
