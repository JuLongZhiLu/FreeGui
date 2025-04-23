cmake_minimum_required(VERSION 3.10)

project(FreeGui LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# example
add_subdirectory(example/createWindow)

# 跨平台配置
if(WIN32)
    message(STATUS "Platform: Windows")
    add_definitions(-DFREEGUI_PLATFORM_WINDOWS)
elseif(UNIX AND NOT APPLE)
    add_definitions(-DFREEGUI_PLATFORM_LINUX)
endif()

# 创建库
# 在add_library部分添加window.cpp
add_library(FreeGui
    src/core/object.cpp
    src/core/signal_slot.cpp
    src/widgets/widget.cpp
    src/widgets/window.cpp # 新增
)

# 包含目录
target_include_directories(FreeGui PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# 安装配置
install(TARGETS FreeGui
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)