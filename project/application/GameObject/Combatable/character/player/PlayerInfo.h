#pragma once
#include <cmath>

#include "math/Vector2.h"
#include "math/Vector3.h"
#include "math/Vector4.h"

#include <nlohmann/json.hpp>

// ---------- ScalarStat: 1つの数値ステータスの集約 ----------
struct ScalarStat 
{
    float baseValue = 0.0f;   // 基礎値（初期値など）
    float increment = 0.0f;   // 強化1回あたりの上昇量
    int upgradeCount = 0;     // 強化回数

    float final() const       // 最終値 = baseValue + increment * upgradeCount
    {
        return baseValue * (1.0f + increment * float(upgradeCount));
    }
};

// ---------- PlayerStats: プレイヤーの各ステータス ----------
struct PlayerStats
{
    float maxHitPoint = 100.0f;
    float hitPoint = 100.0f;   // 現在HP
    ScalarStat knockback{ 1.0f, 1.0f, 0 };
    ScalarStat attackSpeed{ 1.0f, 1.0f, 0 };
    ScalarStat attackDamage{ 1.0f, 1.0f, 0 };
    ScalarStat moveSpeed{ 9.0f, 1.0f, 0 };
    float heal = 30.0f;
    float exp = 0.0f;
};

// ---------- XPProgress: XPのパラメータ+状態+操作を一体化 ----------
struct XPProgress
{
    // params
    float s = 1.0f;    // コストスケール

    // state
    int   level = 1;
    float totalXP = 0.0f;
    float xpIntoLevel = 0.0f;
    float xpToNextThreshold = 0.0f;

    // operations
    static float totalXPForLevel(int N, float s)
    {
        return s * (float(N) * float(N) - 1.0f);
    }

    static float deltaXP(int level, float s)  // 参考：レベル差分
    {
        return s * (2.0f * level + 1.0f);
    }

    void recalc()
    {
        float prev = totalXPForLevel(level, s);
        float next = totalXPForLevel(level + 1, s);
        xpIntoLevel = totalXP - prev;
        xpToNextThreshold = next - prev;
    }

    int addXP(float gained)
    {
        totalXP += gained;
        int ups = 0;
        const float eps = 1e-5f;
        while (totalXP + eps >= totalXPForLevel(level + 1, s)) 
        {
            ++level;
            ++ups;
        }
        recalc();
        return ups;
    }

    static float calibrateS(float xpPerSec, float durationSec, int targetLevelN)
    {
        float B = xpPerSec * durationSec;
        return B / (float(targetLevelN * targetLevelN - 1));
    }
};

// ---------- まとめ ----------
struct PlayerInfo
{
    PlayerStats stats;     // 初期・装備込みの基礎（セーブ/ロード対象）
    XPProgress  xp;
};

// JSON
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ScalarStat, baseValue, increment, upgradeCount)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(PlayerStats, maxHitPoint, hitPoint, knockback, attackSpeed, attackDamage, moveSpeed, heal, exp)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(XPProgress, s, level, totalXP, xpIntoLevel, xpToNextThreshold)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(PlayerInfo, stats, xp)
