cmake_minimum_required(VERSION 3.17)
project(AAMED VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)

# compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 设置默认构建类型为 Release
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

# Debug 构建设置
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")

# Release 构建设置
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
link_libraries(${OpenCV_LIBS})

include_directories("src")

file(GLOB_RECURSE PROJECT_SOURCES "src/*.cpp")
list(REMOVE_ITEM PROJECT_SOURCES "src/main.cpp")

add_executable(AAMEDemo ${PROJECT_SOURCES} "src/main.cpp")

add_library(${PROJECT_NAME} ${PROJECT_SOURCES})

# 根据构建类型添加编译选项
target_compile_options(AAMEDemo PRIVATE $<$<CONFIG:Debug>:-g>)
target_compile_options(${PROJECT_NAME} PRIVATE $<$<CONFIG:Debug>:-g>)

# 链接 OpenCV 库
target_link_libraries(AAMEDemo ${OpenCV_LIBS})
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})

# 输出构建类型信息
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
