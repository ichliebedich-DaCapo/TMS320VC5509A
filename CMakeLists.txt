set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)
cmake_minimum_required(VERSION 3.29)
# 不知道为什么g++编译器测试时会一堆他宝贝的未定义错误
include(Tools/cmake/arm-gcc.cmake)
project(demo C CXX ASM)

# ===================设置工程目录======================
set(APP_DIR Projects/1_debug)

# ===================设置工程目录======================

# =====================设置项目文件=====================
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(TARGET_FILE ${PROJECT_NAME}.elf)
else ()
    set(TARGET_FILE ${PROJECT_NAME}.out)
endif ()

# 生成配置文件
get_filename_component(DIR_NAME ${APP_DIR} NAME)  # 从 "App/x_calc" 提取 "x_calc"
string(REGEX MATCH "^[0-9]+" PREFIX_NUM ${DIR_NAME})  # 匹配开头的连续数字
if(PREFIX_NUM)
    set(PROGRAM_INDEX ${PREFIX_NUM} CACHE INTERNAL "Program Index")  # 提取成功，例如 2
else()
    set(PROGRAM_INDEX 0 CACHE INTERNAL "Program Index")             # 未匹配到数字，例如目录名为 "demo"
endif()
configure_file(${CMAKE_SOURCE_DIR}/Projects/conf/app_conf.h.in ${CMAKE_SOURCE_DIR}/Projects/conf/app_conf.h)

#-------------------------------------------库-------------------------------------------
file(GLOB_RECURSE SOURCES
        "Adapter/GUI/gui.cpp"
        "Algorithm/src/*.cpp"
        "${APP_DIR}/*.c"
        "${APP_DIR}/*.cpp"
        "BSP/src/*.cpp"
        "Core/Drivers/src/*.cpp"
        "Middleware/Library/gui/*.c"
        "Middleware/Library/gui/*.cpp"
)
include_directories(
        "Adapter/GUI"
        "Algorithm/inc"
        ${APP_DIR}
        "Projects"
        "Projects/conf"
        "BSP/inc"
        "Core/Drivers/inc"
        "Middleware/Third_Party/u8g2"
        "Middleware/Library/gui"
)

add_executable(${TARGET_FILE} ${SOURCES})

## 链接库
# 如果不是GCC，那么就链接TI的库

if (NOT CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
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