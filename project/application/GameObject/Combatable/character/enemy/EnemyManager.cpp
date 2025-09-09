#include "EnemyManager.h"

#include "zombie/ZombieEnemy.h"
#include "rushEnemy/RushEnemy.h"
#include "burstEnemy/BurstEnemy.h"
#include "chargeEnemy/ChargeEnemy.h"
#include "application/GameObject/Combatable/character/player/Player.h"
#include "effects/particle/component/single/AccelerationComponent.h"
#include "effects/particle/component/single/ColorFadeOutComponent.h"
#include "effects/particle/component/single/DragComponent.h"
#include "math/VectorColorCodes.h"

void EnemyManager::Initialize(Object3dCommon* object3dCommon, LightManager* lightManager, GameObject* target)
{
	object3dCommon_ = object3dCommon;
	lightManager_ = lightManager;
	target_ = target;
	enemies_.clear();
	enemiesToAdd_.clear();
	
	// 敵キャラクターの出現範囲を設定
	emitRange_ =
	{
		{ -50.0f, 1.0f, -50.0f }, // 最小座標
		{ 50.0f, 1.0f, 50.0f }   // 最大座標
	};

	deathEffect_ = std::make_unique<ParticleEmitter>();
	deathEffect_->Initialize("deathEffect", "./Resources/star.png");
	deathEffect_->SetEmitRate(0.0f);
	deathEffect_->SetInitialLifeTime(1.5f);
	deathEffect_->SetInitialScale(Vector3(4.0f, 25.0f, 4.0f));
	deathEffect_->SetInitialColor(VectorColorCodes::White);
	deathEffect_->SetBillborad(true);
	deathEffect_->SetModelType(ParticleGroup::ParticleType::Plane);
	deathEffect_->AddComponent(std::make_shared<DragComponent>(0.95f)); // 空気抵抗
	deathEffect_->AddComponent(std::make_shared<ColorFadeOutComponent>());
	deathEffect_->AddComponent(std::make_shared<AccelerationComponent>(Vector3(0.0f, 0.3f, 0.0f))); // ゆっくり落ちる
}

void EnemyManager::Update()
{
	for (auto& enemy : enemies_)
	{
		if (enemy)
		{
			enemy->Update(); // 各敵キャラクターの更新
		}
	}

	// 敵キャラクターのリストから死亡した敵を削除
	for (auto it = enemies_.begin(); it != enemies_.end();)
	{
		if (!(*it)->IsAlive())
		{
			(*it)->CallOnDeath(); // 死亡時のコールバックを呼び出す

			// 死亡エフェクト生成
			deathEffect_->Start(
				&(*it)->GetPosition(),
				1,
				0.0f,
				false
			);

			it = enemies_.erase(it); // 死亡した敵を削除
			auto player = dynamic_cast<Player*>(target_);
			player->XPGain(2.0f);
		} 
		else
		{
			++it; // 次の敵へ
		}
	}

	// ループ後にまとめて追加
	for (auto& e : enemiesToAdd_)
	{
		enemies_.emplace_back(std::move(e));
	}
	enemiesToAdd_.clear();

}

void EnemyManager::Draw(CameraManager* camera)
{
	for (auto& enemy : enemies_)
	{
		if (enemy)
		{
			enemy->Draw(camera); // 各敵キャラクターの描画
		}
	}
}

void EnemyManager::AddZombieEnemy(uint32_t count)
{
	for (uint32_t i = 0; i < count; ++i)
	{
		auto enemy = std::make_unique<ZombieEnemy>();
		enemy->Initialize(object3dCommon_, lightManager_, new float(1.0f), target_);
		// 出現位置をランダムに設定
		float x = emitRange_.min_.x + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (emitRange_.max_.x - emitRange_.min_.x)));
		float z = emitRange_.min_.z + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (emitRange_.max_.z - emitRange_.min_.z)));
		enemy->SetPosition(Vector3(x, enemy->GetScale().y, z)); // Y座標は地面に合わせる
		enemies_.emplace_back(std::move(enemy));
	}
}

void EnemyManager::AddRushEnemy(uint32_t count)
{
	for (uint32_t i = 0; i < count; ++i)
	{
		auto enemy = std::make_unique<RushEnemy>();
		enemy->Initialize(object3dCommon_, lightManager_, new float(2.0f), target_);
		// 出現位置をランダムに設定
		float x = emitRange_.min_.x + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (emitRange_.max_.x - emitRange_.min_.x)));
		float z = emitRange_.min_.z + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (emitRange_.max_.z - emitRange_.min_.z)));
		enemy->SetPosition(Vector3(x, enemy->GetScale().y, z)); // Y座標は地面に合わせる
		enemies_.emplace_back(std::move(enemy));
	}
}

void EnemyManager::AddBurstEnemy(uint32_t count)
{
	for (uint32_t i = 0; i < count; ++i)
	{
		auto enemy = std::make_unique<BurstEnemy>();
		// 死亡時のコールバックを設定
		enemy->SetOnDeathCallback([this](EnemyBase* enemy)
			{
				// 分裂して小さいバーストエネミーを生成
				const uint32_t splitCount = 5; // 分裂する数
				for (uint32_t j = 0; j < splitCount; ++j)
				{
					auto smallEnemy = std::make_unique<BurstEnemy>();
					smallEnemy->Initialize(object3dCommon_, lightManager_, new float(1.3f), target_);
					// 分裂した敵の位置を少しずらす
					float angle = (360.0f / splitCount) * j;
					float radians = angle * (3.14159f / 180.0f);
					float offsetX = cosf(radians) * 1.0f; // 半径1.0fの円周上に配置
					float offsetZ = sinf(radians) * 1.0f;
					smallEnemy->SetPosition(enemy->GetPosition() + Vector3(offsetX, 0.0f, offsetZ));
					smallEnemy->SetScale(enemy->GetScale() * 0.7f); // 小さくする
					enemiesToAdd_.emplace_back(std::move(smallEnemy));
				}
			});

		enemy->Initialize(object3dCommon_, lightManager_, new float(1.1f), target_);
		// 出現位置をランダムに設定
		float x = emitRange_.min_.x + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (emitRange_.max_.x - emitRange_.min_.x)));
		float z = emitRange_.min_.z + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (emitRange_.max_.z - emitRange_.min_.z)));
		enemy->SetPosition(Vector3(x, enemy->GetScale().y, z)); // Y座標は地面に合わせる
		enemies_.emplace_back(std::move(enemy));
	}
}

void EnemyManager::AddChargeEnemy(uint32_t count)
{
	for (uint32_t i = 0; i < count; ++i)
	{
		auto enemy = std::make_unique<ChargeEnemy>();
		enemy->Initialize(object3dCommon_, lightManager_, new float(1.0f), target_);
		// 出現位置をランダムに設定
		float x = emitRange_.min_.x + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (emitRange_.max_.x - emitRange_.min_.x)));
		float z = emitRange_.min_.z + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (emitRange_.max_.z - emitRange_.min_.z)));
		enemy->SetPosition(Vector3(x, enemy->GetScale().y, z)); // Y座標は地面に合わせる
		enemies_.emplace_back(std::move(enemy));
	}
}

void EnemyManager::SetEnemyData(const std::vector<GameObjectInfo>& data)
{
	enemyData_ = data;
	enemies_.clear();
}

void EnemyManager::Clear()
{
	enemies_.clear(); // 敵キャラクターのリストをクリア
	enemiesToAdd_.clear(); // 分裂用の一時リストをクリア
}