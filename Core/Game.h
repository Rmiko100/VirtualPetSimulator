#pragma once

#include "Models/Pet.h"
#include "Systems/TimeSystem.h"

#include <memory>
#include <string>

enum class Species { Cat = 1, Dog, Rabbit };
enum class Action { Feed = 2, Play, Rest, Clean, Stroke, Wait };

// 管理一次游戏中的唯一一只宠物，供界面查询状态、提交操作。
class Game {
public:
    void adopt(Species species, const std::string& name);
    const Pet* getPet() const;
    const TimeSystem& getTime() const;
    std::string perform(Action action);
    bool isRunning() const;
    void quit();

private:
    std::string finishTurn(InteractionResult result, int minutes, int reward, bool resting = false);
    std::unique_ptr<Pet> pet;
    TimeSystem time;
    bool running = true;
};
