#include "TitleScene.h"

// audio
#include "audio/Audio.h"
// scene
#include "engine/scene/manager/SceneManager.h"
// editor
#include "externals/imgui/imgui.h"
// math
#include "math/VectorColorCodes.h"
// graphics
#include "manager/graphics/LineManager.h"
#include "manager/effect/PostProcessManager.h"
// app
#include "application/GameObject/component/collision/CollisionManager.h"
#include "../../ResultUI/Result.h"
// components
#include "effects/particle/component/group/MaterialColorComponent.h"
#include "effects/particle/component/group/UVTranslateComponent.h"
#include "effects/particle/component/single/AccelerationComponent.h"
#include "effects/particle/component/single/BounceComponent.h"
#include "effects/particle/component/single/ColorFadeOutComponent.h"
#include "effects/particle/component/single/DragComponent.h"
#include "effects/particle/component/single/GravityComponent.h"
#include "effects/particle/component/single/OrbitComponent.h"
#include "effects/particle/component/single/RandomInitialVelocityComponent.h"
#include "effects/particle/component/single/RotationComponent.h"
#include "effects/particle/component/single/ScaleOverLifetimeComponent.h"
#include "input/Input.h"
#include "time/TimeManager.h"
#include "time/TimerManager.h"

static Vector3 fallHeartPos = Vector3{ 2.0f, 2.0f, 0.0f };
static Vector3 glitchPos = Vector3{ -2.0f, 2.0f, 0.0f };
static Vector3 startPos = Vector3{ 0.0f, 2.0f, 0.0f };
static Vector3 mordeVFXPos = Vector3{ 0.0f, 0.0f, 0.0f };
static Vector3 origin = Vector3{ 0.0f, 3.0f, 0.0f };

void CreateTimer()
{
	auto timer = std::make_unique<Timer>("test", 10.0f, DeltaTimeType::RealDeltaTime);
	timer->SetOnStart([]() {
		TimeManager::GetInstance().SetTimeScale(0.2f);
					  });
	timer->SetOnFinish([]() {
		TimeManager::GetInstance().SetTimeScale(1.0f);
					   });
	TimerManager::GetInstance().AddTimer(std::move(timer));
}

void TitleScene::Initialize()
{
	Audio::GetInstance()->LoadWave("title_bgm", "bgm/title.wav", SoundGroup::BGM);
	Audio::GetInstance()->LoadWave("tap", "se/tap.wav", SoundGroup::BGM);
	// 音声の再生
	Audio::GetInstance()->PlayWave("title_bgm", true);
	Audio::GetInstance()->SetVolume("title_bgm", 0.1f);

	//当たり判定マネージャーの初期化
	CollisionManager::GetInstance()->Initialize();

	// デバッグカメラの生成
	debugCamera_ = std::make_unique<DebugCamera>();
	debugCamera_->Initialize(sceneManager_->GetCameraManager()->GetActiveCamera());
	debugCamera_->Start();

	// オービットカメラの生成
	orbitCamera_ = std::make_unique<OrbitCameraWork>();
	orbitCamera_->Initialize(sceneManager_->GetCameraManager()->GetActiveCamera());
	orbitCamera_->SetPositionOffset(Vector3(0.0f, 3.0f, 0.0f));

	// スカイドームの生成
	skydome_ = std::make_unique<GameObject>();
	skydome_->Initialize(sceneManager_->GetObject3dCommon(), sceneManager_->GetLightManager());
	skydome_->SetModel("skydome");
	skydome_->SetScale(Vector3(0.3f, 0.3f, 0.3f));

	// ロゴの生成
	logo_ = std::make_unique<GameObject>();
	logo_->Initialize(sceneManager_->GetObject3dCommon(), sceneManager_->GetLightManager());
	logo_->SetModel("logo");
	logo_->SetPosition(Vector3(0.0f, 5.0f, 0.0f));

	// 衝突判定を2Dモードに変更
	CollisionManager::GetInstance()->SetCollisionDimension(CollisionDimension::Mode2D);
	// 2Dモード時の衝突判定面をXz平面に設定
	CollisionManager::GetInstance()->SetCollisionPlane(CollisionPlane::XZ);

	// フェードの初期化
	fade_ = std::make_unique<Fade>();
	fade_->Initialize("./Resources/firstFade.png", sceneManager_->GetSpriteCommon());

	// UIの生成
	//ui_ = std::make_unique<TitleUI>(sceneManager_->GetSpriteCommon());
	//ui_->Initialize();Resources/UI/titleUI
	ui_ = std::make_unique<Sprite>();
	ui_->Initialize(sceneManager_->GetSpriteCommon(), "Resources/UI/titleUI.png");
	ui_->SetPosition({ 0.0f,0.0f });
	ui_->SetSize({ 1280.0f,720.0f });

	player_ = std::make_unique<Player>();
	player_->Initialize(
		sceneManager_->GetObject3dCommon(),
		sceneManager_->GetLightManager()
	);
	player_->SetPosition({0.0f,0.0f,0.0f});

	orbitCamera_->Start(
		&player_->GetPosition(),
		35.0f,
		0.5f
	);

	playerParticle_ = std::make_unique<ParticleEmitter>();
	playerParticle_->Initialize("titlePlayerParticle", "./Resources/circle2.png");
	playerParticle_->SetInitialColor(VectorColorCodes::Salmon);
	playerParticle_->SetInitialScale(Vector3(0.01f,0.01f,0.01f));
	playerParticle_->SetEmitRate(0.2f);
	playerParticle_->SetEmitRange(Vector3(-30.0f, 0.0f, -30.0f), Vector3(30.0f, 1.0f, 30.0f));
	playerParticle_->SetRandomScale(true);
	playerParticle_->SetRandomScaleRange(AABB(Vector3(0.001f, 0.001f, 0.001f), Vector3(0.2f, 0.2f, 0.2f)));
	playerParticle_->SetInitialLifeTime(1.0f);
	playerParticle_->SetBillborad(true);
	// コンポーネントの追加
	playerParticle_->AddComponent(std::make_shared<DragComponent>(0.95f));
	playerParticle_->AddComponent(std::make_shared<ColorFadeOutComponent>());
	playerParticle_->AddComponent(std::make_shared<AccelerationComponent>(Vector3(0.0f, 0.3f, 0.0f)));
	playerParticle_->Start(
		&player_->GetPosition(),
		15,
		0.0f,
		true
	);

	zoneEffect_ = std::make_unique<ParticleEmitter>();
	zoneEffect_->Initialize("zoneEffect", "./Resources/zone.png");
	zoneEffect_->SetEmitRange({}, {}); // 座標の位置で発生
	zoneEffect_->SetInitialLifeTime(0.04f);
	zoneEffect_->SetEmitRate(0.0f);
	zoneEffect_->SetBillborad(false);
	zoneEffect_->SetInitialScale(Vector3(60.0f, 25.0f, 60.0f));
	zoneEffect_->SetInitialRotation(Vector3{ std::numbers::pi_v<float>, 0.0f, 0.0f }); //　地面に倒す角度にする
	zoneEffect_->SetInitialColor(VectorColorCodes::Firebrick - Vector4(0.0f, 0.0f, 0.0f, 0.7f));
	// 円柱を使用する
	zoneEffect_->SetModelType(ParticleGroup::ParticleType::Cylinder);
	// Y軸回転を加える
	zoneEffect_->AddComponent(std::make_shared<UVTranslateComponent>(Vector3{ -0.03f, 0.0f, 0.0f })); // UVを毎フレーム大きくずらす

	zoneEffect_->Start(
		Vector3(),
		1,
		0.0f,
		true
	);

	Result::GetInstance()->Init();
}

void TitleScene::Finalize()
{
	CollisionManager::GetInstance()->Finalize();

	// Audioの停止
	Audio::GetInstance()->StopWave("title_bgm");
}

void TitleScene::Update()
{
	if (Input::GetInstance()->TriggerKey(DIK_SPACE))
	{
		Audio::GetInstance()->PlayWave("tap", false);
		nextScene_ = true;
		fade_->Start(
			FadeType::FadeOut,
			2.0f
		);
	}
	if (nextScene_ && !fade_->IsActive())
	{
		sceneManager_->ChangeScene("GAMEPLAY");
	}

	// ImGuiの描画
	DrawImGui();

	// 前フレームの位置を更新
	CollisionManager::GetInstance()->UpdatePreviousPositions();

	fade_->Update();

	orbitCamera_->Update();

	// プレイヤーの浮遊処理（コンポーネント化推奨）
	static float floatTime_ = 0.0f;
	floatTime_ += TimeManager::GetInstance().GetDeltaTime();
	float floatHeight = 0.1f; // 浮遊の高さ
	float floatSpeed = 2.0f;  // 浮遊の速さ
	Vector3 playerPos = player_->GetPosition();
	playerPos.y = std::sin(floatTime_ * floatSpeed) * floatHeight;
	player_->SetPosition(playerPos);

	// 衝突判定開始
	CollisionManager::GetInstance()->CheckCollisions();

	ui_->Update();
}

void TitleScene::Draw3D()
{
	// グリッドの描画
	LineManager::GetInstance()->DrawGrid(
		300.0f,
		5.0f,
		VectorColorCodes::Black
	);

	skydome_->Draw(sceneManager_->GetCameraManager());
	player_->Draw(sceneManager_->GetCameraManager());
	logo_->Draw(sceneManager_->GetCameraManager());
}

void TitleScene::Draw2D()
{
	ui_->Draw();
	fade_->Draw();
}


void TitleScene::DrawImGui()
{
#ifdef _DEBUG
	ImGui::Begin("TitleScene");

	if (ImGui::Button("create timer"))
	{
		CreateTimer();
	}

	if (ImGui::Button("Fade In"))
	{
		fade_->Start(
			FadeType::FadeIn,
			2.0f,
			VectorColorCodes::White
		);
	}
	ImGui::SameLine();
	if (ImGui::Button("Fade Out"))
	{
		fade_->Start(
			FadeType::FadeOut,
			2.0f
		);
	}

	static bool updateDebugCamera = false;
	ImGui::SameLine();
	ImGui::Checkbox("update debug", &updateDebugCamera);
	if (updateDebugCamera)
	{
		debugCamera_->Update();
	}

	// プレイヤーの平行光源の情報
	Vector3 lightDir = player_->GetObject3d()->GetLightingDirection();
	if (ImGui::DragFloat3("Light Direction", &lightDir.x, 0.1f, -1.0f, 1.0f))
	{
		player_->GetObject3d()->SetLightingDirection(lightDir);
	}

#pragma region PostProcess
	ImGui::SeparatorText("PostProcess");
	if (ImGui::CollapsingHeader("GrayScale"))
	{
		static bool isGrayScale = false;
		if (ImGui::Checkbox("enable", &isGrayScale))
		{
			sceneManager_->GetPostProcessManager()->grayscaleEffect_->SetEnabled(isGrayScale);
		}
		float intensity = sceneManager_->GetPostProcessManager()->grayscaleEffect_->GetIntensity();
		ImGui::DragFloat("GrayScale Intensity", &intensity, 0.01f, 0.0f, 1.0f);
		sceneManager_->GetPostProcessManager()->grayscaleEffect_->SetIntensity(intensity);
	}
	if (ImGui::CollapsingHeader("Vignette"))
	{
		static bool isVignette = false;
		if (ImGui::Checkbox("enable", &isVignette))
		{
			sceneManager_->GetPostProcessManager()->vignetteEffect_->SetEnabled(isVignette);
		}
		float intensity = sceneManager_->GetPostProcessManager()->vignetteEffect_->GetIntensity();
		ImGui::DragFloat("Vignette Intensity", &intensity, 0.01f, 0.0f, 1.0f);
		sceneManager_->GetPostProcessManager()->vignetteEffect_->SetIntensity(intensity);
		float radius = sceneManager_->GetPostProcessManager()->vignetteEffect_->GetRadius();
		ImGui::DragFloat("Vignette Radius", &radius, 0.01f, 0.0f, 1.0f);
		sceneManager_->GetPostProcessManager()->vignetteEffect_->SetRadius(radius);
		float softness = sceneManager_->GetPostProcessManager()->vignetteEffect_->GetSoftness();
		ImGui::DragFloat("Vignette Softness", &softness, 0.01f, 0.0f, 1.0f);
		sceneManager_->GetPostProcessManager()->vignetteEffect_->SetSoftness(softness);
		Vector3 color = sceneManager_->GetPostProcessManager()->vignetteEffect_->GetColor();
		ImGui::ColorEdit3("Vignette Color", &color.x);
		sceneManager_->GetPostProcessManager()->vignetteEffect_->SetColor(color);
	}
	if (ImGui::CollapsingHeader("Noise"))
	{
		static bool isNoise = false;
		if (ImGui::Checkbox("enable", &isNoise))
		{
			sceneManager_->GetPostProcessManager()->noiseEffect_->SetEnabled(isNoise);
		}
		float intensity = sceneManager_->GetPostProcessManager()->noiseEffect_->GetIntensity();
		ImGui::DragFloat("Noise Intensity", &intensity, 0.01f, 0.0f, 1.0f);
		sceneManager_->GetPostProcessManager()->noiseEffect_->SetIntensity(intensity);
		float time = sceneManager_->GetPostProcessManager()->noiseEffect_->GetTime();
		ImGui::DragFloat("Noise Time", &time, 0.01f, 0.0f, 10.0f);
		sceneManager_->GetPostProcessManager()->noiseEffect_->SetTime(time);
		float grainSize = sceneManager_->GetPostProcessManager()->noiseEffect_->GetGrainSize();
		ImGui::DragFloat("Noise Grain Size", &grainSize, 0.01f, 0.0f, 10.0f);
		sceneManager_->GetPostProcessManager()->noiseEffect_->SetGrainSize(grainSize);
		float luminanceAffect = sceneManager_->GetPostProcessManager()->noiseEffect_->GetLuminanceAffect();
		ImGui::DragFloat("Noise Luminance Affect", &luminanceAffect, 0.01f, 0.0f, 1.0f);
		sceneManager_->GetPostProcessManager()->noiseEffect_->SetLuminanceAffect(luminanceAffect);
	}
	if (ImGui::CollapsingHeader("CRT"))
	{
		static  bool isEnabled = false;
		static bool isCrt = false; // CRTエフェクトの有効/無効
		static bool isScanline = false;
		static bool isDistortion = false;
		static bool isChromAberration = false;

		if (ImGui::Checkbox("enable", &isEnabled))
		{
			sceneManager_->GetPostProcessManager()->crtEffect_->SetEnabled(isEnabled);
		}
		if (ImGui::Checkbox("Crt", &isCrt))
		{
			sceneManager_->GetPostProcessManager()->crtEffect_->SetCrtEnabled(isCrt);
		}
		ImGui::SameLine();
		if (ImGui::Checkbox("Scanline", &isScanline))
		{
			sceneManager_->GetPostProcessManager()->crtEffect_->SetScanlineEnabled(isScanline);
		}
		ImGui::SameLine();
		if (ImGui::Checkbox("Distortion", &isDistortion))
		{
			sceneManager_->GetPostProcessManager()->crtEffect_->SetDistortionEnabled(isDistortion);
		}
		ImGui::SameLine();
		if (ImGui::Checkbox("ChromAberration", &isChromAberration))
		{
			sceneManager_->GetPostProcessManager()->crtEffect_->SetChromaticAberrationEnabled(isChromAberration);
		}
		float scanlineIntensity = sceneManager_->GetPostProcessManager()->crtEffect_->GetScanlineIntensity();
		ImGui::DragFloat("Scanline Intensity", &scanlineIntensity, 0.01f, 0.0f, 1.0f);
		sceneManager_->GetPostProcessManager()->crtEffect_->SetScanlineIntensity(scanlineIntensity);
		float scanlineCount = sceneManager_->GetPostProcessManager()->crtEffect_->GetScanlineCount();
		ImGui::DragFloat("Scanline Count", &scanlineCount, 10.0f, 0.0f, 1000.0f);
		sceneManager_->GetPostProcessManager()->crtEffect_->SetScanlineCount(scanlineCount);
		float distortionStrength = sceneManager_->GetPostProcessManager()->crtEffect_->GetDistortionStrength();
		ImGui::DragFloat("Distortion Strength", &distortionStrength, 0.01f, 0.0f, 10.0f);
		sceneManager_->GetPostProcessManager()->crtEffect_->SetDistortionStrength(distortionStrength);
		float chromAberrationOffset = sceneManager_->GetPostProcessManager()->crtEffect_->GetChromaticAberrationOffset();
		ImGui::DragFloat("Chromatic Aberration Offset", &chromAberrationOffset, 0.01f, 0.0f, 10.0f);
		sceneManager_->GetPostProcessManager()->crtEffect_->SetChromaticAberrationOffset(chromAberrationOffset);
	}
#pragma endregion

	ImGui::End();

#endif
}
