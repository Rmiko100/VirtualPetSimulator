#pragma once

#include "Pet.h"

// 兔子沿用基类的属性与构造函数，重写种类名称和玩耍行为。
class Rabbit : public Pet {
public:
    using Pet::Pet;
    std::string getSpecies() const override;
    std::string play() override;
};
