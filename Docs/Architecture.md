# 第一阶段架构

## 一次玩耍如何完成

假设玩家领养了“小橘”。在主菜单输入 `3` 时，`ConsoleUI` 将编号转换为 `Action::Play`，交给 `Game::perform()`。`Game` 调用 `pet->play()`。由于 `pet` 实际指向 `Cat`，C++ 会执行 `Cat::play()`。该函数把心情增量 15、精力消耗 8 和动作文字传给 `Pet::tryPlay()`。精力足够时，模型同时更新心情与精力，返回动作文字。界面显示结果并再次展示主菜单。

```text
main
 └─ ConsoleUI::run(game)
     ├─ 领养 → Game::adopt(species, name) → 创建 Cat / Dog / Rabbit
     ├─ 状态 → Game::getPet() → 通过 const Pet* 读取各项属性
     ├─ 互动 → Game::perform(action)
     │         ├─ feed() / rest() / clean() / stroke()
     │         └─ pet->play() → 具体种类的 play() → Pet::tryPlay()
     └─ 退出 → Game::quit() → 菜单循环结束
```

## 文件与职责

| 文件 | 职责 |
| --- | --- |
| `main.cpp` | 创建 `Game` 和 `ConsoleUI`，调用 `ui.run(game)`。 |
| `CMakeLists.txt` | 列出编译单元，设置 C++17 与 MSVC 的 UTF-8 编译选项，生成控制台可执行文件。 |
| `README.md` | 提供运行方法、编译步骤、菜单规则和手工验收示例。 |
| `Models/Pet.h` | 声明抽象基类、私有属性、只读查询、通用互动和纯虚接口。 |
| `Models/Pet.cpp` | 处理名称空白，初始化数值，定义析构函数和查询函数，实施互动规则及属性边界检查。 |
| `Models/Cat.h`、`Models/Cat.cpp` | 头文件声明猫类及重写接口，实现文件返回“猫咪”并定义玩毛线球的增益和消耗。 |
| `Models/Dog.h`、`Models/Dog.cpp` | 头文件声明狗类及重写接口，实现文件返回“狗狗”并定义玩飞盘的增益和消耗。 |
| `Models/Rabbit.h`、`Models/Rabbit.cpp` | 头文件声明兔子类及重写接口，实现文件返回“兔子”并定义跳跃玩耍的增益和消耗。 |
| `Core/Game.h` | 声明种类和操作枚举，以及领养、查询、互动和退出接口。 |
| `Core/Game.cpp` | 按所选种类创建对象，管理其生命周期，按玩家的操作调用模型。 |
| `UI/ConsoleUI.h` | 声明控制台界面及其运行入口。 |
| `UI/ConsoleUI.cpp` | 负责中文输入输出、领养流程、菜单和状态显示、非法输入重试，并将有效操作交给游戏层。 |
| `Systems/README.md` | 记录商店、背包、随机事件、存档等系统的预留位置和接入方式。 |
| `Data/` | 为后续数据文件预留的空目录。 |
| `Docs/Architecture.md` | 记录本阶段的调用关系、设计理由与下一阶段建议。 |
| `bin/VirtualPetSimulator.exe` | 已从本项目构建的 Windows x64 Release 程序，可直接运行。 |

## 面向对象知识点

- **封装**　`Pet` 的属性全部为 `private`。界面只能通过 `const Pet*` 读取状态。修改走 `feed()` 等行为函数，可变的状态数值统一通过 `std::clamp` 限制在 0～100。`tryPlay()` 先检查精力再修改，避免扣掉精力却没有完成玩耍。
- **继承**　`Cat`、`Dog`、`Rabbit` 公有继承 `Pet`。三个类沿用基类构造函数和喂食、休息、清洁、抚摸的实现。
- **多态**　`Pet::play()` 和 `Pet::getSpecies()` 是纯虚函数，派生类使用 `override` 实现它们。`Game` 只在领养时选择具体类型。之后同一次 `pet->play()` 调用会随实际对象执行不同的玩耍行为。
- **构造与析构**　`Pet` 构造函数通过初始化列表设置名称和全部初值。`Game` 持有 `std::unique_ptr<Pet>`，生命周期结束时自动释放所领养的宠物。虚析构函数保证通过基类指针释放对象时，先调用派生类析构函数，再调用基类析构函数。

## 下一阶段建议

建议先用一个最小 2D 窗口复用现有逻辑。领养按钮调用 `Game::adopt()`，玩耍按钮调用 `Game::perform(Action::Play)`，状态面板从 `getPet()` 读取最新数值。可以用同一只猫首次玩耍后“心情 95、精力 62”的结果，对照验收两种界面。后续系统可按课程进度逐个加入 `Systems/`。
