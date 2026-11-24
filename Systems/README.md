# 游戏系统

`TimeSystem.h` 和 `TimeSystem.cpp` 实现回合制时钟。游戏从第 1 天 08:00 开始，时钟按累计分钟数计算天、小时和分钟，并通过 `getTimeString()` 提供统一的时间显示格式。

`Game` 在成功互动或等待时调用 `advanceTime()`，并协调宠物每小时的属性变化。`ConsoleUI` 只读当前时间。后续图形界面可以复用同一套游戏接口。
