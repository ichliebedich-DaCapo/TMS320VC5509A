set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)
cmake_minimum_required(VERSION 3.29)
# 不知道为什么g++编译器测试时会一堆他宝贝的未定义错误
project(demo C CXX ASM)
include(Tools/cmake/arm-gcc.cmake)


# ===================设置工程目录======================
set(APP_DIR App/demo)
# ===================设置工程目录======================

#-------------------------------------------库-------------------------------------------
file(GLOB_RECURSE SOURCES
        "${APP_DIR}/*.c"
        "BSP/src/*.c"
        "Core/Drivers/src/*.c"
)
include_directories([[E:/Tools/Develop/ToolsKits/TI/C5500_Code_Generation_Tools4.4.1/include]]
        "App"
        "App/conf"
        "BSP/inc"
        "Core/Drivers/inc"
)

add_executable(${TARGET_FILE} ${SOURCES})

## 链接库
# 如果不是GCC，那么就链接TI的库
if(NOT CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
target_include_directories(${TARGET_FILE} PRIVATE
        "E:/Tools/Develop/ToolsKits/TI/C5500_Code_Generation_Tools4.4.1/include"
)
target_link_directories(${TARGET_FILE} PRIVATE
        "E:/Tools/Develop/ToolsKits/TI/C5500_Code_Generation_Tools4.4.1/lib"
)

endif ()

# 添加自定义命令：
#add_custom_command(TARGET ${TARGET_FILE} POST_BUILD
#        COMMENT "Building "
#)