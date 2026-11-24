#include "Core/Game.h"
#include "Models/Cat.h"
#include "Systems/TimeSystem.h"

#include <array>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>

namespace {
// Release 构建也执行每项检查。失败时报告具体规则。
void check(bool ok, const char* rule) {
    if (!ok) {
        throw std::runtime_error(rule);
    }
}

std::array<long long, 8> state(const Pet& p) {
    return {p.getSatiety(), p.getHealth(), p.getMood(), p.getEnergy(),
            p.getHygiene(), p.getIntimacy(), p.getLevel(), p.getExperience()};
}

void checkNoEffect(Game& game, Action action) {
    const auto before = state(*game.getPet());
    const auto time = game.getTime().getTimeString();
    game.perform(action);
    check(before == state(*game.getPet()) && time == game.getTime().getTimeString(),
          "失败或无效操作改变了属性、经验或时间");
}

void clockRules() {
    TimeSystem time;
    check(time.getTimeString() == "第 1 天 08:00", "初始时间");
    time.advanceTime(59);
    check(time.getHour() == 8 && time.getMinute() == 59, "整点之前");
    time.advanceTime(1);
    check(time.getHour() == 9 && time.getMinute() == 0, "跨小时");
    time.advanceTime(2 * 1440 + 15 * 60 + 7);
    check(time.getDay() == 4 && time.getTimeString() == "第 4 天 00:07", "多日与午夜");
    bool rejected = false;
    try {
        time.advanceTime(-1);
    } catch (const std::invalid_argument&) {
        rejected = true;
    }
    time.advanceTime(0);
    check(rejected && time.getTimeString() == "第 4 天 00:07", "负分钟或零分钟");
}

void turnsAndRest() {
    Game game;
    game.adopt(Species::Cat, "小橘");
    const Pet& p = *game.getPet();
    game.perform(Action::Feed);
    game.perform(Action::Clean);
    check(game.getTime().getTimeString() == "第 1 天 08:30" && p.getSatiety() == 70
          && p.getHygiene() == 100 && p.getExperience() == 20, "两个 15 分钟回合");
    game.perform(Action::Play);
    check(game.getTime().getTimeString() == "第 1 天 09:00" && p.getSatiety() == 65
          && p.getMood() == 93 && p.getEnergy() == 59 && p.getHygiene() == 98
          && p.getExperience() == 40, "短回合累计一小时");
    game.perform(Action::Wait);
    check(game.getTime().getTimeString() == "第 1 天 10:00" && p.getEnergy() == 56
          && p.getExperience() == 40, "等待一小时");
    game.perform(Action::Stroke);
    game.perform(Action::Rest);
    check(game.getTime().getTimeString() == "第 1 天 12:05" && p.getEnergy() == 86
          && p.getSatiety() == 50 && p.getMood() == 92 && p.getHygiene() == 92
          && p.getIntimacy() == 5 && p.getExperience() == 50, "跨两个整点休息");
    checkNoEffect(game, static_cast<Action>(99));
    game.quit();
    check(!game.isRunning(), "退出状态");
    checkNoEffect(game, Action::Wait);
}

void playRules() {
    struct Example { Species species; int mood; int energy; const char* activity; };
    const Example examples[] = {{Species::Cat, 95, 62, "毛线球"},
                                {Species::Dog, 100, 55, "飞盘"},
                                {Species::Rabbit, 92, 64, "跳跃"}};
    for (const auto& e : examples) {
        Game game;
        game.adopt(e.species, "伙伴");
        const std::string result = game.perform(Action::Play);
        const Pet& pet = *game.getPet();
        check(result.find(e.activity) != std::string::npos && pet.getMood() == e.mood
              && pet.getEnergy() == e.energy && pet.getExperience() == 20
              && game.getTime().getTimeString() == "第 1 天 08:30", "各宠物的多态玩耍");
    }
    Game game;
    game.adopt(Species::Cat, "小橘");
    for (int i = 0; i < 7; ++i) {
        game.perform(Action::Play);
    }
    check(game.getPet()->getEnergy() == 5, "低精力测试前置状态");
    checkNoEffect(game, Action::Play);
}

void noOpRules() {
    struct Example { Action action; int repetitions; };
    const Example examples[] = {{Action::Feed, 3}, {Action::Clean, 1},
                                {Action::Rest, 1}, {Action::Stroke, 20}};
    for (const auto& e : examples) {
        Game game;
        game.adopt(Species::Dog, "小白");
        for (int i = 0; i < e.repetitions; ++i) {
            game.perform(e.action);
        }
        const Pet& pet = *game.getPet();
        if (e.action == Action::Feed) check(pet.getSatiety() == 100, "喂食上限");
        if (e.action == Action::Clean) check(pet.getHygiene() == 100, "清洁上限");
        if (e.action == Action::Rest) check(pet.getEnergy() == 100, "精力上限");
        if (e.action == Action::Stroke) {
            check(pet.getMood() == 100 && pet.getIntimacy() == 100, "抚摸上限");
        }
        checkNoEffect(game, e.action);
        if (e.action == Action::Stroke) {
            game.perform(Action::Wait);
            const auto previous = pet.getExperience();
            game.perform(Action::Stroke);
            check(pet.getExperience() == previous + 5, "抚摸仅改善心情时仍有效");
        }
    }
}

void healthRules() {
    Cat cat("花花");
    for (int hour = 1; hour <= 29; ++hour) {
        if (hour <= 14) cat.feed();
        cat.applyHourlyDecay(false);
    }
    check(cat.getSatiety() == 20 && cat.getHygiene() == 22 && cat.getHealth() == 100,
          "饱腹度恰为 20 时健康不减少");
    cat.applyHourlyDecay(false);
    check(cat.getHealth() == 95 && cat.getHygiene() == 20, "清洁度恰为 20 时只扣饥饿惩罚");
    cat.applyHourlyDecay(false);
    check(cat.getHealth() == 87, "饥饿和清洁度惩罚叠加");
    Game game;
    game.adopt(Species::Rabbit, "小雪");
    for (int i = 0; i < 50; ++i) game.perform(Action::Wait);
    const Pet& p = *game.getPet();
    check(state(p) == std::array<long long, 8>{0, 0, 0, 0, 0, 0, 1, 0}, "属性下界");
    std::string warnings;
    for (const auto& warning : p.getStatusWarnings()) warnings += warning;
    for (const char* keyword : {"饥饿", "疲惫", "低落", "清洁", "较差", "危急"}) {
        check(warnings.find(keyword) != std::string::npos, "低属性和危急状态提醒");
    }
    game.perform(Action::Feed);
    check(game.isRunning() && p.getSatiety() == 20 && p.getExperience() == 10,
          "危急状态仍能互动");
}

void growthRules() {
    Cat cat("花花");
    cat.addExperience(99);
    check(cat.getLevel() == 1 && cat.getRequiredExperience() == 100, "未达升级门槛");
    cat.addExperience(1);
    check(cat.getLevel() == 2 && cat.getExperience() == 0, "经验恰好达到升级门槛");
    cat.addExperience(899);
    check(cat.getLevel() == 4 && cat.getExperience() == 399
          && cat.getGrowthStage() == GrowthStage::Baby, "连续升级与经验结余");
    cat.addExperience(1);
    check(cat.getLevel() == 5 && cat.getGrowthStage() == GrowthStage::Juvenile, "等级 5");
    cat.addExperience(3499);
    check(cat.getLevel() == 9 && cat.getExperience() == 899
          && cat.getGrowthStage() == GrowthStage::Juvenile, "等级 9");
    cat.addExperience(1);
    check(cat.getLevel() == 10 && cat.getGrowthStage() == GrowthStage::Adult, "等级 10");
    // 已有可观结余时一次加入 int 最大值，核对升级前后总经验守恒。
    cat.addExperience(999);
    cat.addExperience(std::numeric_limits<int>::max());
    const long long level = cat.getLevel();
    check(level * (level - 1) * 50 + cat.getExperience() ==
              5499LL + std::numeric_limits<int>::max(), "大额经验没有溢出或丢失");
}

void growthMessages() {
    Game game;
    game.adopt(Species::Cat, "小橘");
    const Pet& p = *game.getPet();
    for (int i = 0; i < 1000 && p.getLevel() < 10; ++i) {
        const int before = p.getLevel();
        const auto result = game.perform(p.getEnergy() >= 8 ? Action::Play : Action::Rest);
        if (p.getLevel() != before) {
            check(result.find("升级") != std::string::npos, "升级提示");
            if (p.getLevel() == 5 || p.getLevel() == 10) {
                check(result.find(p.getGrowthStageName()) != std::string::npos, "成长提示");
            }
        }
    }
    check(p.getLevel() == 10, "通过真实互动进入成年期");
}
} // namespace

int main() {
    try {
        clockRules();
        turnsAndRest();
        playRules();
        noOpRules();
        healthRules();
        growthRules();
        growthMessages();
        std::cout << "通过 7 组核心规则测试。\n";
    } catch (const std::exception& error) {
        std::cerr << "测试失败：" << error.what() << '\n';
        return 1;
    }
}
