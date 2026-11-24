#include "Cat.h"

std::string Cat::getSpecies() const {
    return "猫咪";
}

std::string Cat::play() {
    return tryPlay(15, 8, "玩起了毛线球");
}
