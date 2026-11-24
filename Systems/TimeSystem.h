#pragma once

#include "Core/GameRules.h"

#include <cstdint>
#include <string>

// 只保存从第 1 天零点起算的分钟数。日历各部分由同一数值计算。
class TimeSystem {
public:
    // 返回本次跨过的整点数量，供 Game 逐小时结算。
    int advanceTime(int minutes);
    std::int64_t getDay() const;
    int getHour() const;
    int getMinute() const;
    std::string getTimeString() const;

private:
    std::int64_t totalMinutes = GameRules::startHour * GameRules::minutesPerHour;
};
