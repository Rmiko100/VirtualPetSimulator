#include "Dog.h"

std::string Dog::getSpecies() const {
    return "狗狗";
}

std::string Dog::play() {
    return tryPlay(20, 15, "玩起了飞盘");
}
