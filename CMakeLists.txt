set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)
cmake_minimum_required(VERSION 3.29)
# 不知道为什么g++编译器测试时会一堆他宝贝的未定义错误
include(User/Tools/cmake/arm-gcc.cmake)
project(demo C CXX ASM)

# =====================设置项目文件=====================
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(TARGET_FILE ${PROJECT_NAME}.elf)
else ()
    set(TARGET_FILE ${PROJECT_NAME}.out)
endif ()


#-------------------------------------------库-------------------------------------------
file(GLOB_RECURSE SOURCES
        "Utils/*.cpp"
        "Core/*.cpp"
        "BSP/src/*.cpp"
        "Drivers/src/*.cpp"
        "Middleware/GUI/*.c"
        "Middleware/GUI/*.cpp"
)
include_directories(
        "Utils"
        "Core"
        "Core/ui"
        "BSP/inc"
        "Drivers/inc"
        "Middleware/GUI"
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