#pragma once

#include "Pet.h"

// 狗沿用基类的属性与构造函数，重写种类名称和玩耍行为。
class Dog : public Pet {
public:
    using Pet::Pet;
    std::string getSpecies() const override;
    InteractionResult play() override;
};
