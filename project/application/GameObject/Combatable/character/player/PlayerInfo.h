#pragma once
#include <cmath>

#include "math/Vector2.h"
#include "math/Vector3.h"
#include "math/Vector4.h"

// json
#include <nlohmann/json.hpp>

// プレイヤーのステータス
struct PlayerStatus
{
	float hitPoint = 100.0f;	// 体力
	float knockback = 1.0f;		// ノックバック距離
	float attackSpeed = 1.0f;	// 攻撃速度
	float attackDamage = 1.0f;	// 攻撃力
	float moveSpeed = 1.0f;		// 移動速度
	float exp = 0.0f;			// 取得済み経験値
};

// 各ステータスの倍率
struct PlayerStatusMultiplier
{
	float knockback = 1.0f;		// ノックバック倍率
	float attackSpeed = 1.0f;	// 攻撃速度倍率
	float attackDamage = 1.0f;	// 攻撃力倍率
	float moveSpeed = 1.0f;		// 移動速度倍率
};

// ステータスボーナスの倍率（加算）
struct PlayerStatusBonus
{
	float knockback = 0.15f;	// ノックバック倍率
	float attackSpeed = 0.15f;	// 攻撃速度倍率
	float attackDamage = 0.15f;	// 攻撃力倍率
	float moveSpeed = 0.15f;	// 移動速度倍率
	float heal = 30.0f;			// 体力回復量
};

struct XPParams
{
    float s = 1.0f;          // スケール係数
};

inline float DeltaXP(int level /*ℓ>=1*/, const XPParams& p)
{
    float d = p.s * (2.0f * level + 1.0f);
    return d;
}

inline float TotalXPForLevel(int N /*到達Lv, N>=1*/, const XPParams& p)
{
    // Lv1到達に必要な累積0、LvN到達に s*(N^2-1)
    return p.s * (float(N) * float(N) - 1.0f);
}

struct XPState
{
    int   level = 1;                // 現在Lv（整数）
    float totalXP = 0.0f;           // 累積獲得XP（float）
    float xpIntoLevel = 0.0f;       // そのLv内の進捗（UI用）
    float xpToNextThreshold = 0.0f; // 次Lvまでの必要量（差分）
};

inline void RecalcProgress(const XPParams& p, XPState& st)
{
    float prevTot = TotalXPForLevel(st.level, p);
    float nextTot = TotalXPForLevel(st.level + 1, p);

    st.xpIntoLevel = st.totalXP - prevTot;
    st.xpToNextThreshold = nextTot - prevTot;
}

inline int AddXP(XPState& st, const XPParams& p, float gainedXP)
{
    st.totalXP += gainedXP;
    int ups = 0;
    const float eps = 1e-5f;

    // レベルアップを必要なだけ繰り返す（分裂やボーナスで一気に増えるケースに対応）
    while (st.totalXP + eps >= TotalXPForLevel(st.level + 1, p))
    {
        st.level++;
        ups++;
    }
    RecalcProgress(p, st);
    return ups;
}

// 目標Lvに合わせて s を推定
inline float CalibrateS(float lambda_xp_per_sec, float duration_sec, int targetLevelN)
{
    float B = lambda_xp_per_sec * duration_sec;
    return B / (float(targetLevelN * targetLevelN - 1));
}

// プレイヤー情報
struct PlayerInfo
{
	PlayerStatus status;
	PlayerStatusMultiplier multiplier;
	PlayerStatusBonus addBonus;
    XPParams xpParams;
    XPState xpState;
};

// Jsonシリアライズ用のマクロ
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(PlayerStatus,
	hitPoint, knockback, attackSpeed, attackDamage, moveSpeed, exp)

	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(PlayerStatusMultiplier,
		knockback, attackSpeed, attackDamage, moveSpeed)

	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(PlayerStatusBonus,
		knockback, attackSpeed, attackDamage, moveSpeed, heal)

	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(XPParams,
		s)

	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(XPState,
		level, totalXP, xpIntoLevel, xpToNextThreshold)

	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(PlayerInfo, status, multiplier, addBonus)
