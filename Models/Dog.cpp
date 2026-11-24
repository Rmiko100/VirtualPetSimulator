#include "Dog.h"

#include "Core/GameRules.h"

std::string Dog::getSpecies() const {
    return "狗狗";
}

InteractionResult Dog::play() {
    return tryPlay(GameRules::dogPlayMoodGain, GameRules::dogPlayEnergyCost, "玩起了飞盘");
}
