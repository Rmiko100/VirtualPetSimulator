#include "Cat.h"

#include "Core/GameRules.h"

std::string Cat::getSpecies() const {
    return "猫咪";
}

InteractionResult Cat::play() {
    return tryPlay(GameRules::catPlayMoodGain, GameRules::catPlayEnergyCost, "玩起了毛线球");
}
