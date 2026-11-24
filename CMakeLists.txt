cmake_minimum_required(VERSION 3.16)
project(VirtualPetSimulator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
if(MSVC)
    add_compile_options(/utf-8 /W4 /permissive-)
    # 让控制台程序及核心测试使用同一套静态运行库。
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

# 游戏和测试共享核心代码。核心模块仅依赖 C++ 标准库。
add_library(VirtualPetCore STATIC
    Models/Pet.cpp    Models/Pet.h
    Models/Cat.cpp    Models/Cat.h
    Models/Dog.cpp    Models/Dog.h
    Models/Rabbit.cpp Models/Rabbit.h
    Core/Game.cpp     Core/Game.h
    Core/GameRules.h
    Systems/TimeSystem.cpp Systems/TimeSystem.h
)
target_include_directories(VirtualPetCore PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

add_executable(VirtualPetSimulator main.cpp UI/ConsoleUI.cpp UI/ConsoleUI.h)
target_link_libraries(VirtualPetSimulator PRIVATE VirtualPetCore)

include(CTest)
if(BUILD_TESTING)
    add_executable(Phase2Tests Tests/Phase2Tests.cpp)
    target_link_libraries(Phase2Tests PRIVATE VirtualPetCore)
    add_test(NAME Phase2.Core COMMAND Phase2Tests)
    add_test(NAME Phase2.Console COMMAND "${CMAKE_COMMAND}"
        "-DGAME_EXE=$<TARGET_FILE:VirtualPetSimulator>" -P "${CMAKE_CURRENT_SOURCE_DIR}/Tests/ConsoleFlow.cmake")
endif()
