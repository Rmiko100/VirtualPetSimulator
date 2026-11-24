#include "TimeSystem.h"

#include <iomanip>
#include <limits>
#include <sstream>
#include <stdexcept>

int TimeSystem::advanceTime(int minutes) {
    if (minutes < 0) {
        throw std::invalid_argument("推进时间的分钟数不能为负。");
    }
    if (totalMinutes > std::numeric_limits<std::int64_t>::max() - minutes) {
        throw std::overflow_error("游戏时间超出可表示范围。");
    }
    const auto previousHour = totalMinutes / GameRules::minutesPerHour;
    totalMinutes += minutes;
    return static_cast<int>(totalMinutes / GameRules::minutesPerHour - previousHour);
}

std::int64_t TimeSystem::getDay() const {
    return totalMinutes / GameRules::minutesPerHour / GameRules::hoursPerDay + 1;
}

int TimeSystem::getHour() const {
    return static_cast<int>(totalMinutes / GameRules::minutesPerHour % GameRules::hoursPerDay);
}

int TimeSystem::getMinute() const {
    return static_cast<int>(totalMinutes % GameRules::minutesPerHour);
}

std::string TimeSystem::getTimeString() const {
    std::ostringstream text;
    text << "第 " << getDay() << " 天 " << std::setfill('0') << std::setw(2) << getHour()
         << ':' << std::setw(2) << getMinute();
    return text.str();
}
