#include "Rabbit.h"

std::string Rabbit::getSpecies() const {
    return "兔子";
}

std::string Rabbit::play() {
    return tryPlay(12, 6, "开心地跳跃玩耍");
}
