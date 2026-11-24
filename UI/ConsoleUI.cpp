#include "ConsoleUI.h"

#include "../Core/Game.h"
#include "../Models/Pet.h"

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

namespace {
// 控制台以宽字符读取中文，再交给游戏使用 UTF-8。重定向输入直接读取 UTF-8 文本。
bool readLine(std::string& line) {
    std::cout.flush();
#ifdef _WIN32
    const HANDLE input = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode = 0;
    if (GetConsoleMode(input, &mode)) {
        std::wstring wideLine;
        do {
            wchar_t chunk[256];
            DWORD count = 0;
            if (!ReadConsoleW(input, chunk, 256, &count, nullptr) || count == 0) {
                return false;
            }
            wideLine.append(chunk, count);
            if (wideLine.front() == L'\x1a') {  // Ctrl+Z 后回车
                return false;
            }
        } while (wideLine.back() != L'\n');

        wideLine.pop_back();
        if (!wideLine.empty() && wideLine.back() == L'\r') {
            wideLine.pop_back();
        }
        line.clear();
        if (wideLine.empty()) {
            return true;
        }
        const int length = static_cast<int>(wideLine.size());
        const int bytes = WideCharToMultiByte(CP_UTF8, 0, wideLine.data(), length,
                                              nullptr, 0, nullptr, nullptr);
        if (bytes == 0) {
            return false;
        }
        line.resize(bytes);
        return WideCharToMultiByte(CP_UTF8, 0, wideLine.data(), length, line.data(),
                                   bytes, nullptr, nullptr) != 0;
    }
#endif
    return static_cast<bool>(std::getline(std::cin, line));
}

// 按整行解析编号，避免把“2abc”或“2 3”当成一次合法操作。
bool readChoice(const char* prompt, int minChoice, int maxChoice, int& choice) {
    std::string line;
    while (true) {
        std::cout << prompt;
        if (!readLine(line)) {
            return false;
        }
        std::istringstream input(line);
        char extra;
        if ((input >> choice) && !(input >> extra) && choice >= minChoice && choice <= maxChoice) {
            return true;
        }
        std::cout << "无效选项，请输入 " << minChoice << " 到 " << maxChoice << " 的整数。\n";
    }
}

void showTitle() {
    std::cout << "================================\n"
                 "       Virtual Pet Simulator\n"
                 "           虚拟宠物养成\n"
                 "================================\n";
}

void showPetSummary(const Pet& pet) {
    std::cout << "当前宠物：" << pet.getName() << '\n'
              << "宠物种类：" << pet.getSpecies() << '\n'
              << "当前等级：" << pet.getLevel() << '\n';
}
}  // namespace

bool ConsoleUI::adoptPet(Game& game) {
    showTitle();
    std::cout << "\n请选择想领养的宠物\n[1] 猫咪\n[2] 狗狗\n[3] 兔子\n";
    int choice = 0;
    if (!readChoice("请输入种类编号：", 1, 3, choice)) {
        return false;
    }

    std::string name;
    while (true) {
        std::cout << "请为宠物命名：";
        if (!readLine(name)) {
            return false;
        }
        try {
            game.adopt(static_cast<Species>(choice), name);
            std::cout << "领养成功！\n";
            return true;
        } catch (const std::invalid_argument& error) {
            std::cout << error.what() << '\n';
        }
    }
}

void ConsoleUI::showMenu(const Pet& pet) const {
    std::cout << '\n';
    showTitle();
    showPetSummary(pet);
    std::cout << "\n[1] 查看宠物状态\n[2] 喂食\n[3] 玩耍\n"
                 "[4] 休息\n[5] 清洁\n[6] 抚摸\n[0] 退出游戏\n\n";
}

void ConsoleUI::showStatus(const Pet& pet) const {
    std::cout << "\n---------- 宠物状态 ----------\n";
    showPetSummary(pet);
    std::cout << "饱腹度：" << pet.getSatiety() << '\n'
              << "健康值：" << pet.getHealth() << '\n'
              << "心情值：" << pet.getMood() << '\n'
              << "精力值：" << pet.getEnergy() << '\n'
              << "清洁度：" << pet.getHygiene() << '\n'
              << "经验值：" << pet.getExperience() << '\n'
              << "亲密度：" << pet.getIntimacy() << '\n';
}

void ConsoleUI::run(Game& game) {
#ifdef _WIN32
    const UINT oldCodePage = GetConsoleOutputCP();
    DWORD mode = 0;
    const bool changedCodePage = GetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), &mode)
        && oldCodePage != 0 && oldCodePage != CP_UTF8 && SetConsoleOutputCP(CP_UTF8);
#endif
    if (game.isRunning() && !game.getPet() && !adoptPet(game)) {
        game.quit();
    }
    while (game.isRunning()) {
        const Pet& pet = *game.getPet();
        showMenu(pet);
        int choice = 0;
        if (!readChoice("请输入操作：", 0, 6, choice) || choice == 0) {
            game.quit();
        } else if (choice == 1) {
            showStatus(pet);
        } else {
            std::cout << game.perform(static_cast<Action>(choice)) << '\n';
        }
    }
    std::cout << "\n已退出游戏。\n" << std::flush;
#ifdef _WIN32
    if (changedCodePage) {
        SetConsoleOutputCP(oldCodePage);
    }
#endif
}
