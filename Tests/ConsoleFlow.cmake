cmake_minimum_required(VERSION 3.16)

# 使用真实程序验证整行输入、状态查询、等待与退出的回合边界。
set(input_file "${CMAKE_CURRENT_BINARY_DIR}/Phase2ConsoleFlow.input")
file(WRITE "${input_file}" "1\n\n小橘\n2abc\n1\n7\n1\n0\n7\n")
execute_process(COMMAND "${GAME_EXE}" INPUT_FILE "${input_file}" OUTPUT_VARIABLE output
                RESULT_VARIABLE status TIMEOUT 10 ENCODING UTF-8)
file(REMOVE "${input_file}")
if(NOT status STREQUAL "0")
    message(FATAL_ERROR "控制台程序退出异常：${status}")
endif()

foreach(expected IN ITEMS "小橘" "无效选项" "已退出游戏" "幼年期" "0 / 100" "[7] 等待一小时")
    string(FIND "${output}" "${expected}" position)
    if(position EQUAL -1)
        message(FATAL_ERROR "输出缺少：${expected}")
    endif()
endforeach()

# 忽略同一时间的重复显示，只比较玩家实际看到的时钟变化。
string(REGEX MATCHALL "时间：第 [0-9]+ 天 [0-9]+:[0-9]+" times "${output}")
set(changes "")
set(previous "")
foreach(current IN LISTS times)
    if(NOT current STREQUAL previous)
        list(APPEND changes "${current}")
        set(previous "${current}")
    endif()
endforeach()
if(NOT changes STREQUAL "时间：第 1 天 08:00;时间：第 1 天 09:00")
    message(FATAL_ERROR "状态查看、非法输入或退出错误推进时间：${times}")
endif()
