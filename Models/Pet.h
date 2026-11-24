#pragma once

#include <string>

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
    int getExperience() const;
    int getIntimacy() const;

    virtual std::string getSpecies() const = 0;
    virtual std::string play() = 0;
    void feed();
    void rest();
    void clean();
    void stroke();

protected:
    // 先检查精力，再一起更新心情与精力，保证被拒绝时属性保持原值。
    std::string tryPlay(int moodGain, int energyCost, const char* activity);

private:
    std::string name;
    int satiety;
    int health;
    int mood;
    int energy;
    int hygiene;
    int level;
    int experience;
    int intimacy;
};
