#include "Core/Game.h"
#include "UI/ConsoleUI.h"

int main() {
    Game game;
    ConsoleUI ui;
    ui.run(game);
    return 0;
}
