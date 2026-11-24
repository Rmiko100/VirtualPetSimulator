cmake_minimum_required(VERSION 3.16)
project(VirtualPetSimulator LANGUAGES CXX)

add_executable(VirtualPetSimulator
    main.cpp
    Models/Pet.cpp    Models/Pet.h
    Models/Cat.cpp    Models/Cat.h
    Models/Dog.cpp    Models/Dog.h
    Models/Rabbit.cpp Models/Rabbit.h
    Core/Game.cpp     Core/Game.h
    UI/ConsoleUI.cpp  UI/ConsoleUI.h
)

set_target_properties(VirtualPetSimulator PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)
target_include_directories(VirtualPetSimulator PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")

if(MSVC)
    target_compile_options(VirtualPetSimulator PRIVATE /utf-8 /W4 /permissive-)
    # 静态链接运行库，方便把 Release 可执行文件带到其他 Windows 电脑。
    set_property(TARGET VirtualPetSimulator PROPERTY
        MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()
