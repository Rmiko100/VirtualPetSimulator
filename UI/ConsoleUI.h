#pragma once

class Game;
class Pet;

// 控制台界面负责读取玩家指令，并展示游戏返回的状态和结果。
class ConsoleUI {
public:
    void run(Game& game);

private:
    bool adoptPet(Game& game);
    void showMenu(const Pet& pet) const;
    void showStatus(const Pet& pet) const;
};
