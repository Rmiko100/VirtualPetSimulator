#include "Rabbit.h"

#include "Core/GameRules.h"

std::string Rabbit::getSpecies() const {
    return "兔子";
}

InteractionResult Rabbit::play() {
    return tryPlay(GameRules::rabbitPlayMoodGain, GameRules::rabbitPlayEnergyCost, "开心地跳跃玩耍");
}
