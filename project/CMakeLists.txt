cmake_minimum_required(VERSION 3.10)

if(WIN32)
    set(CMAKE_SYSTEM_VERSION "10.0.18362.0")
endif()

SET(CMAKE_TOOLCHAIN_FILE  "${CMAKE_CURRENT_SOURCE_DIR}/../thirdparty_install/vcpkg/scripts/buildsystems/vcpkg.cmake")

project(drawing-tool)
set(CMAKE_CXX_STANDARD 11)

# 设置输出目录为顶层 bin 文件夹
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin")

add_subdirectory(src)








