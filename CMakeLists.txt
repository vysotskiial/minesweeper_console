cmake_minimum_required(VERSION 3.1)
project(minesweeper VERSION 1.0)

find_package(GTK2 REQUIRED)
find_package(GLUT REQUIRED)
find_package(OpenGL REQUIRED)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(minesweeper src/mine_drawer.cpp src/mine_board.cpp src/main.cpp)

target_include_directories(minesweeper PRIVATE include ${GLUT_INCLUDE_DIRS} ${GTK2_INCLUDE_DIRS})
target_link_libraries(minesweeper ${GLUT_LIBRARY} ${OPENGL_LIBRARIES} ${GTK2_LIBRARIES})
