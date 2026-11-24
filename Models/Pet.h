#pragma once

#include <cstdint>
#include <string>
#include <vector>

enum class GrowthStage { Baby, Juvenile, Adult };

// Game 用 changed 决定是否结算本次互动的时间和经验。
struct InteractionResult {
    bool changed;
    std::string message;
};

// 所有宠物共享的属性和互动规则。派生类提供种类名称和玩耍方式。
class Pet {
public:
    explicit Pet(const std::string& petName);
    virtual ~Pet();

    const std::string& getName() const;
    int getSatiety() const;
    int getHealth() const;
    int getMood() const;
    int getEnergy() const;
    int getHygiene() const;
    int getLevel() const;
    std::int64_t getExperience() const;
    int getIntimacy() const;

    std::int64_t getRequiredExperience() const;
    GrowthStage getGrowthStage() const;
    std::string getGrowthStageName() const;
    std::vector<std::string> getStatusWarnings() const;
    void addExperience(int amount);
    void applyHourlyDecay(bool resting);

    virtual std::string getSpecies() const = 0;
    virtual InteractionResult play() = 0;
    InteractionResult feed();
    InteractionResult rest();
    InteractionResult clean();
    InteractionResult stroke();

protected:
    // 先检查精力，再一起更新心情与精力，保证被拒绝时属性保持原值。
    InteractionResult tryPlay(int moodGain, int energyCost, const char* activity);

private:
    std::string name;
    int satiety;
    int health;
    int mood;
    int energy;
    int hygiene;
    int level;
    std::int64_t experience;
    int intimacy;
};
