#pragma once

#include "../../../base/GameObjectInfo.h"
#include "base/EnemyBase.h"
#include "math/AABB.h"

class LightManager;
class Object3dCommon;

class EnemyManager
{
public:
	
	void Initialize(Object3dCommon* object3dCommon, LightManager* lightManager, GameObject* target);
	
	void Update();
	
	void Draw(CameraManager* camera);
	
	const std::vector<std::unique_ptr<EnemyBase>>& GetEnemies() const { return enemies_; }
	
	void AddZombieEnemy(uint32_t count);

	void AddRushEnemy(uint32_t count);

	void AddBurstEnemy(uint32_t count);

	void AddChargeEnemy(uint32_t count);

	void SetEnemyData(const std::vector<GameObjectInfo>& data);
	
	void SetTarget(GameObject* target) { target_ = target; }
	
	void Clear();

private:

	Object3dCommon* object3dCommon_ = nullptr; // 3Dオブジェクト共通処理
	LightManager* lightManager_ = nullptr; // ライトマネージャー
	
	GameObject* target_ = nullptr; // ターゲット（プレイヤーなど）
	
	AABB emitRange_ = {};
	
	// 敵リスト
	std::vector<std::unique_ptr<EnemyBase>> enemies_;
	// 分裂用の一時リスト
	std::vector<std::unique_ptr<EnemyBase>> enemiesToAdd_;
	
	// 敵データ
	std::vector<GameObjectInfo> enemyData_;

};

