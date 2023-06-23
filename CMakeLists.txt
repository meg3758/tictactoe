cmake_minimum_required(VERSION 3.22)
project(tictactoe)

set(CMAKE_CXX_STANDARD 23)

add_executable(tictactoe main.cpp)
set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR C:/SFML/lib/cmake/SFML)
find_package(SFML COMPONENTS system window graphics network audio REQUIRED)
include_directories(C:SFML/include)
target_link_libraries(tictactoe sfml-system sfml-window sfml-graphics sfml-audio)