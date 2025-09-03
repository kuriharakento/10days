#include "EnemyManager.h"

#include "zombie/ZombieEnemy.h"
#include "rushEnemy/RushEnemy.h"

void EnemyManager::Initialize(Object3dCommon* object3dCommon, LightManager* lightManager, GameObject* target)
{
	object3dCommon_ = object3dCommon;
	lightManager_ = lightManager;
	target_ = target;
	enemies_.clear();
	
	// 敵キャラクターの出現範囲を設定
	emitRange_ =
	{
		{ -50.0f, 1.0f, -50.0f }, // 最小座標
		{ 50.0f, 1.0f, 50.0f }   // 最大座標
	};

}

void EnemyManager::Update()
{
	for (auto& enemy : enemies_)
	{
		enemy->Update(); // 各敵キャラクターの更新
	}

	// 敵キャラクターのリストから死亡した敵を削除
	for (auto it = enemies_.begin(); it != enemies_.end();)
	{
		if (!(*it)->IsAlive())
		{
			it = enemies_.erase(it); // 死亡した敵を削除
		} 
		else
		{
			++it; // 次の敵へ
		}
	}
}

void EnemyManager::Draw(CameraManager* camera)
{
	for (auto& enemy : enemies_)
	{
		enemy->Draw(camera); // 各敵キャラクターの描画
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

void EnemyManager::SetEnemyData(const std::vector<GameObjectInfo>& data)
{
	enemyData_ = data;
	enemies_.clear();
}

void EnemyManager::Clear()
{
	enemies_.clear(); // 敵キャラクターのリストをクリア
}