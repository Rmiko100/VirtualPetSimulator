#include "Core/Game.h"
#include "Core/GameRules.h"

#include "Models/Cat.h"
#include "Models/Dog.h"
#include "Models/Rabbit.h"

#include <stdexcept>

void Game::adopt(Species species, const std::string& name) {
    if (!running || pet) {
        throw std::logic_error("当前无法领养宠物。");
    }

    // 仅创建时选择具体类型，后续互动统一通过 Pet 接口调用。
    switch (species) {
    case Species::Cat:
        pet = std::make_unique<Cat>(name);
        break;
    case Species::Dog:
        pet = std::make_unique<Dog>(name);
        break;
    case Species::Rabbit:
        pet = std::make_unique<Rabbit>(name);
        break;
    default:
        throw std::invalid_argument("请选择有效的宠物种类。");
    }
}

const Pet* Game::getPet() const {
    return pet.get();
}

const TimeSystem& Game::getTime() const {
    return time;
}

std::string Game::perform(Action action) {
    if (!running) {
        return "游戏已结束。";
    }
    if (!pet) {
        return "请先领养一只宠物。";
    }

    switch (action) {
    case Action::Feed:
        return finishTurn(pet->feed(), GameRules::feedMinutes, GameRules::feedExperience);
    case Action::Play:
        return finishTurn(pet->play(), GameRules::playMinutes, GameRules::playExperience);
    case Action::Rest:
        return finishTurn(pet->rest(), GameRules::restMinutes, GameRules::restExperience, true);
    case Action::Clean:
        return finishTurn(pet->clean(), GameRules::cleanMinutes, GameRules::cleanExperience);
    case Action::Stroke:
        return finishTurn(pet->stroke(), GameRules::strokeMinutes, GameRules::strokeExperience);
    case Action::Wait:
        // 等待的目的就是推进时间，在属性均为零时也可以进行。
        return finishTurn({true, "等待了一小时。"}, GameRules::waitMinutes, GameRules::waitExperience);
    default:
        return "无法识别这项操作。";
    }
}

std::string Game::finishTurn(InteractionResult result, int minutes, int reward, bool resting) {
    if (!result.changed) {
        return result.message;
    }

    const int hours = time.advanceTime(minutes);
    for (int i = 0; i < hours; ++i) {
        pet->applyHourlyDecay(resting);
    }

    const int previousLevel = pet->getLevel();
    const GrowthStage previousStage = pet->getGrowthStage();
    pet->addExperience(reward);
    if (reward > 0) {
        result.message += " 获得 " + std::to_string(reward) + " 点经验。";
    }
    if (pet->getLevel() != previousLevel) {
        result.message += " 升级至 " + std::to_string(pet->getLevel()) + " 级！";
    }
    if (pet->getGrowthStage() != previousStage) {
        result.message += " 进入";
        result.message += pet->getGrowthStageName();
        result.message += "！";
    }
    return result.message;
}

bool Game::isRunning() const {
    return running;
}

void Game::quit() {
    running = false;
}
