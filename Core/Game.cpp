#include "Core/Game.h"

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

std::string Game::perform(Action action) {
    if (!running) {
        return "游戏已结束。";
    }
    if (!pet) {
        return "请先领养一只宠物。";
    }

    switch (action) {
    case Action::Feed:
        pet->feed();
        return "喂食完成。";
    case Action::Play:
        return pet->play();
    case Action::Rest:
        pet->rest();
        return "休息完成。";
    case Action::Clean:
        pet->clean();
        return "清洁完成。";
    case Action::Stroke:
        pet->stroke();
        return "抚摸完成。";
    default:
        return "无法识别这项操作。";
    }
}

bool Game::isRunning() const {
    return running;
}

void Game::quit() {
    running = false;
}
