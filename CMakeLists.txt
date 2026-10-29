cmake_minimum_required(VERSION 3.14)
project(terminal-2048 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options(/utf-8 /W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

add_library(game_core src/Board.cpp)
target_include_directories(game_core PUBLIC include)

add_executable(2048 src/main.cpp src/Game.cpp src/Terminal.cpp)
target_link_libraries(2048 PRIVATE game_core)

enable_testing()
add_executable(test_board tests/test_board.cpp)
target_link_libraries(test_board PRIVATE game_core)
add_test(NAME board_rules COMMAND test_board)
