#pragma once

class Game;

// 控制台界面负责读取玩家指令，并展示游戏返回的状态和结果。
class ConsoleUI {
public:
    void run(Game& game);

private:
    bool adoptPet(Game& game);
    void showMenu() const;
    void showStatus(const Game& game) const;
};
