#pragma once
class Result
{
public:
    static Result* GetInstance();
    ~Result() = default;
private:
    // シングルトンインスタンス
    // ※外部から生成・コピー・ムーブできないようにする
    Result() {};
    Result(const Result&) = delete;               // コピーコンストラクタ禁止
    Result& operator=(const Result&) = delete;    // コピー代入禁止
    Result(Result&&) = delete;                    // ムーブコンストラクタ禁止
    Result& operator=(Result&&) = delete;         // ムーブ代入禁止

public:
    void Init();

    void SetLifeTime(float time) { lifeTime_ = time; }
    void SetLevel(int level) { level_ = level; }
    void IncrementEnemyNum() { enemyNum_++; }

    float GetLifeTime() { return lifeTime_; }
    int GetLevel() { return level_; }
    int GetEnemyNum() { return enemyNum_; }


private:
    float lifeTime_ = 0.0f;
    int level_ = 1;
    int enemyNum_ = 0;
};

