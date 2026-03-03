cmake_minimum_required(VERSION 3.25)

project(MyApp LANGUAGES CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(SDL3 REQUIRED)
find_package(SDL3_ttf REQUIRED)

add_executable(output
    src/main.cpp
    src/event_handling.cpp
    src/drawing.cpp
    src/texture.cpp
    src/texture_manager.cpp
    src/board.cpp
    src/game_logic.cpp
)

target_include_directories(output PRIVATE ${SDL3_INCLUDE_DIRS})
target_link_libraries(output PRIVATE ${SDL3_LIBRARIES} SDL3_ttf::SDL3_ttf)
