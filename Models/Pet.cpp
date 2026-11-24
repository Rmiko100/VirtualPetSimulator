#include "Pet.h"

#include <algorithm>
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
    return std::clamp(value, 0, 100);
}
} // namespace

Pet::Pet(const std::string& petName)
    : name(trimName(petName)), satiety(50), health(100), mood(80), energy(70),
      hygiene(80), level(1), experience(0), intimacy(0) {}

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

int Pet::getExperience() const {
    return experience;
}

int Pet::getIntimacy() const {
    return intimacy;
}

void Pet::feed() {
    satiety = bounded(satiety + 20);
}

void Pet::rest() {
    energy = bounded(energy + 30);
}

void Pet::clean() {
    hygiene = bounded(hygiene + 25);
}

void Pet::stroke() {
    intimacy = bounded(intimacy + 5);
    mood = bounded(mood + 5);
}

std::string Pet::tryPlay(int moodGain, int energyCost, const char* activity) {
    if (energy < energyCost) {
        return "精力不足，先让宠物休息吧。";
    }
    mood = bounded(mood + moodGain);
    energy = bounded(energy - energyCost);
    return name + activity + "。";
}
