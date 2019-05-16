cmake_minimum_required(VERSION 3.13.0 FATAL_ERROR) # What is FATAL_ERROR

set (CMAKE_C_COMPILER clang)
set (CMAKE_CXX_COMPILER clang++)
set (CMAKE_CXX_STANDARD 17)

set (CMAKE_EXPORT_COMPILE_COMMANDS true)

# output path
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

include_directories(include)
link_directories(lib)

project (BossGame)
set (TARGET_NAME BossGame)

# Works now, although  main is included also from src subdirectories
add_executable(BossGame src/main.cpp)
add_subdirectory(src)

# Link glew statically
add_definitions(-DGLEW_STATIC)

target_link_libraries(BossGame fmt libglfw3 libglew32 opengl32 gdi32)

# std::filesystem library needs tobe linked separately
target_link_libraries(BossGame stdc++fs)