#include "Result.h"


Result* Result::GetInstance() 
{
    static Result instance;
    return &instance;
}

void Result::Init()
{
    lifeTime_ = 0.0f;
    level_ = 1;
    enemyNum_ = 0;
}