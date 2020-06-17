cmake_minimum_required(VERSION 3.1)
project(minesweeper VERSION 1.0)

find_package(GLUT REQUIRED)
find_package(OpenGL REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_WIN32_EXECUTABLE true)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(minesweeper src/mine_drawer.cpp src/mine_board.cpp src/main.cpp)

target_compile_options(minesweeper PRIVATE
	$<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
	-Wall -Wextra>
	$<$<CXX_COMPILER_ID:MSVC>:
	/W3>)

target_include_directories(minesweeper PRIVATE include ${GLUT_INCLUDE_DIR})
target_link_libraries(minesweeper ${GLUT_LIBRARY} ${OPENGL_LIBRARIES})
