# 第二阶段架构 · 时间与成长

## 类之间的调用关系

```text
main → ConsoleUI::run(game)
         ├─ 领养 → Game::adopt() → 构造 Cat / Dog / Rabbit
         ├─ 查看 → Game::getTime()、Game::getPet() → 读取时间及属性
         ├─ 操作 → Game::perform(action)
         │          ├─ Pet 的通用互动 / 具体种类的 play()
         │          └─ Game::finishTurn(result, minutes, reward, resting)
         │               ├─ TimeSystem::advanceTime() → 本回合跨过的整点数
         │               ├─ Pet::applyHourlyDecay(resting) × 整点数
         │               └─ Pet::addExperience() → 比较回合前后的等级和阶段
         └─ 退出 → Game::quit()
```

当 `Game` 收到玩耍操作时，通过 `std::unique_ptr<Pet>` 调用虚函数 `play()`。猫、狗、兔子各自提供心情收益、精力消耗和动作文字，共用 `Pet::tryPlay()` 先检查精力。模型返回 `InteractionResult`，分别携带互动是否生效及反馈文字。`Game` 依据布尔值决定是否结算，不需要分析中文字符串。

等待由 `Game` 接收，直接推进一小时。退出、查看和非法菜单输入均不会调用结算函数。控制台只能从 `getTime()` 取得 `const TimeSystem&`。未来的图形界面可以调用相同的 `Game` 接口。

## 时间和属性由谁保存

| 文件与类型 | 负责的状态或操作 |
| --- | --- |
| `Systems/TimeSystem.h`、`.cpp` | 以 64 位整数记录从第 1 天零点起算的分钟数。天、时、分和显示文字均由该值推导。推进 0 分钟有效，负数抛出 `std::invalid_argument`。 |
| `Core/Game.h`、`.cpp` | 每局持有一只宠物、一个时钟和运行标志。接收操作，协调属性直接变化、时间推进、逐小时衰减及经验奖励。 |
| `Core/GameRules.h` | 集中初始值、属性上限、动作消耗、经验奖励、警戒阈值、成长等级等参数，便于在一个位置调整平衡。 |
| `Models/Pet.h`、`.cpp` | 保存名称和私有属性，实施边界检查，处理健康损失、经验和阶段判断，提供动态状态提示。 |
| `Models/Cat`、`Dog`、`Rabbit` 的 `.h` 和 `.cpp` | 继承 `Pet`，各自重写种类名称和玩耍行为。共同规则由基类执行。 |
| `UI/ConsoleUI.h`、`.cpp` | 领养、中文整行输入、数字校验、展示完整状态与操作结果。Windows 终端输入继续以宽字符读取并转换为 UTF-8。 |
| `main.cpp` | 构造 `Game` 和 `ConsoleUI`，启动菜单循环。 |
| `CMakeLists.txt` | 将游戏逻辑编为 `VirtualPetCore` 静态库，供菜单程序和核心测试共用同一份实现。 |
| `Tests/Phase2Tests.cpp`、`Tests/ConsoleFlow.cmake` | 分别检查类接口及回合规则、启动可执行程序验证菜单行为，由 CTest 统一运行。 |
| `README.md`、`Systems/README.md`、`Data/` | 使用说明、系统目录说明和预留数据目录。`bin/VirtualPetSimulator.exe` 是可直接运行的 Windows x64 Release 版本。 |

## 三个容易算错的边界

1. **短操作跨整点。** 时间系统以推进前后累计小时数的差计算整点数。08:45 喂食 15 分钟触发一次衰减，08:05 喂食则只把时钟推进到 08:20。没有第二份“待结算分钟”需要与时钟同步。
2. **休息与健康。** 休息先恢复精力，再跨过两个整点。每个整点仍减少饱腹度、心情和清洁度，只跳过精力衰减。每小时根据本次衰减后的数值判断健康损失，两个小时的健康损失可能不同。
3. **有效互动与升级。** 状态封顶时，只要还有一项直接属性改变就算成功。经验按当前等级所需值逐次扣除，例如从 1 级一次获得 350 经验，将连续升至 3 级并留下 50 / 300。阶段由等级即时推导。

## 对应的 C++ 知识点

- **封装和只读访问。** 属性保持 `private`。界面拿到 `const Pet*` 和 `const TimeSystem&`，可以查询状态，同时由 `Game` 统一决定何时执行修改。
- **继承、多态与对象生命周期。** `Cat` 等派生类用 `override` 重写纯虚函数。`Game` 使用 `std::unique_ptr<Pet>` 管理当前对象，配合基类虚析构函数自动释放资源。
- **强类型枚举和派生值。** `enum class GrowthStage` 为幼年、成长、成年提供清楚的取值。`getGrowthStage()` 根据等级计算，升级时只需维护等级和经验两项数据。
- **值类型与编译期常量。** `InteractionResult` 作为小结构体按值返回。`GameRules` 的 `static constexpr` 成员为多个类共享的数值提供有含义的名字。
