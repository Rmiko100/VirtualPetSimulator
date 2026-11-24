#include "Pet.h"

#include "Core/GameRules.h"

#include <algorithm>
#include <limits>
#include <stdexcept>
#include <string_view>

namespace {
// 逐字节跳过 ASCII 空白和 UTF-8 全角空格，保留名字中间的原文。
std::string trimName(const std::string& input) {
    constexpr std::string_view asciiSpaces = " \t\r\n\f\v";
    constexpr std::string_view fullWidthSpace = u8"\u3000";
    std::size_t first = std::string::npos;
    std::size_t end = 0;
    for (std::size_t pos = 0; pos < input.size();) {
        if (asciiSpaces.find(input[pos]) != std::string_view::npos) {
            ++pos;
        } else if (input.compare(pos, fullWidthSpace.size(), fullWidthSpace) == 0) {
            pos += fullWidthSpace.size();
        } else {
            if (first == std::string::npos) {
                first = pos;
            }
            ++pos;
            end = pos;
        }
    }
    if (first == std::string::npos) {
        throw std::invalid_argument("宠物名称不能为空或全是空白。");
    }
    return input.substr(first, end - first);
}

int bounded(int value) {
    return std::clamp(value, GameRules::minStat, GameRules::maxStat);
}
} // namespace

Pet::Pet(const std::string& petName)
    : name(trimName(petName)), satiety(GameRules::initialSatiety), health(GameRules::initialHealth),
      mood(GameRules::initialMood), energy(GameRules::initialEnergy), hygiene(GameRules::initialHygiene),
      level(GameRules::initialLevel), experience(GameRules::initialExperience), intimacy(GameRules::initialIntimacy) {}

Pet::~Pet() = default;

const std::string& Pet::getName() const {
    return name;
}

int Pet::getSatiety() const {
    return satiety;
}

int Pet::getHealth() const {
    return health;
}

int Pet::getMood() const {
    return mood;
}

int Pet::getEnergy() const {
    return energy;
}

int Pet::getHygiene() const {
    return hygiene;
}

int Pet::getLevel() const {
    return level;
}

std::int64_t Pet::getExperience() const {
    return experience;
}

int Pet::getIntimacy() const {
    return intimacy;
}

std::int64_t Pet::getRequiredExperience() const {
    return static_cast<std::int64_t>(level) * GameRules::experiencePerLevel;
}

void Pet::addExperience(int amount) {
    if (amount < 0) {
        throw std::invalid_argument("增加的经验不能为负数。");
    }
    // 先加到宽整数中，再逐级扣除门槛，保留最后剩余的经验。
    std::int64_t remaining = experience + amount;
    while (remaining >= getRequiredExperience()) {
        if (level == std::numeric_limits<int>::max()) {
            throw std::overflow_error("等级超过可表示的范围。");
        }
        remaining -= getRequiredExperience();
        ++level;
    }
    experience = remaining;
}

GrowthStage Pet::getGrowthStage() const {
    if (level >= GameRules::adultLevel) {
        return GrowthStage::Adult;
    }
    if (level >= GameRules::juvenileLevel) {
        return GrowthStage::Juvenile;
    }
    return GrowthStage::Baby;
}

std::string Pet::getGrowthStageName() const {
    switch (getGrowthStage()) {
    case GrowthStage::Baby: return "幼年期";
    case GrowthStage::Juvenile: return "成长期";
    case GrowthStage::Adult: return "成年期";
    }
    throw std::logic_error("未知成长阶段。");
}

std::vector<std::string> Pet::getStatusWarnings() const {
    std::vector<std::string> warnings;
    if (satiety < GameRules::lowStatThreshold) {
        warnings.emplace_back("宠物非常饥饿。");
    }
    if (energy < GameRules::lowStatThreshold) {
        warnings.emplace_back("宠物非常疲惫。");
    }
    if (mood < GameRules::lowStatThreshold) {
        warnings.emplace_back("宠物情绪低落。");
    }
    if (hygiene < GameRules::lowStatThreshold) {
        warnings.emplace_back("宠物需要清洁。");
    }
    if (health < GameRules::lowHealthThreshold) {
        warnings.emplace_back("宠物健康状况较差。");
    }
    if (health == GameRules::minStat) {
        warnings.emplace_back("宠物处于危急状态！");
    }
    return warnings;
}

void Pet::applyHourlyDecay(bool resting) {
    satiety = bounded(satiety - GameRules::hourlySatietyLoss);
    mood = bounded(mood - GameRules::hourlyMoodLoss);
    if (!resting) {
        energy = bounded(energy - GameRules::hourlyEnergyLoss);
    }
    hygiene = bounded(hygiene - GameRules::hourlyHygieneLoss);

    // 健康以本小时衰减后的状态判断，饥饿与清洁度过低的损失可叠加。
    if (satiety < GameRules::lowStatThreshold) {
        health = bounded(health - GameRules::hungerHealthLoss);
    }
    if (hygiene < GameRules::lowStatThreshold) {
        health = bounded(health - GameRules::hygieneHealthLoss);
    }
}

InteractionResult Pet::feed() {
    if (satiety == GameRules::maxStat) {
        return {false, "饱腹度已满，宠物不需要喂食。"};
    }
    satiety = bounded(satiety + GameRules::feedSatietyGain);
    return {true, "喂食完成。"};
}

InteractionResult Pet::rest() {
    if (energy == GameRules::maxStat) {
        return {false, "精力已满，宠物不需要休息。"};
    }
    energy = bounded(energy + GameRules::restEnergyGain);
    return {true, "休息完成。"};
}

InteractionResult Pet::clean() {
    if (hygiene == GameRules::maxStat) {
        return {false, "清洁度已满，宠物不需要清洁。"};
    }
    hygiene = bounded(hygiene + GameRules::cleanHygieneGain);
    return {true, "清洁完成。"};
}

InteractionResult Pet::stroke() {
    if (intimacy == GameRules::maxStat && mood == GameRules::maxStat) {
        return {false, "亲密度和心情已满。"};
    }
    intimacy = bounded(intimacy + GameRules::strokeIntimacyGain);
    mood = bounded(mood + GameRules::strokeMoodGain);
    return {true, "抚摸完成。"};
}

InteractionResult Pet::tryPlay(int moodGain, int energyCost, const char* activity) {
    if (energy < energyCost) {
        return {false, "精力不足，先让宠物休息吧。"};
    }
    mood = bounded(mood + moodGain);
    energy = bounded(energy - energyCost);
    return {true, name + activity + "。"};
}
