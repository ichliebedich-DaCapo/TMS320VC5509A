set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)
cmake_minimum_required(VERSION 3.29)

# 指定CGT
#set(CMAKE_C_COMPILER_WORKS 1)   # 强制绕过编译器检查
# 强制禁用响应文件
#set(CMAKE_C_USE_RESPONSE_FILE_FOR_OBJECTS OFF)
#set(CMAKE_CXX_USE_RESPONSE_FILE_FOR_OBJECTS OFF)
set(CMAKE_C_COMPILER [[E:\Tools\Develop\ToolsKits\TI\C5500_Code_Generation_Tools4.4.1\bin\cl55.exe]])  # TI C55x编译器路径
set(CMAKE_CXX_COMPILER ${CMAKE_C_COMPILER})
set(CMAKE_ASM_COMPILER ${CMAKE_C_COMPILER})

# 项目名称
project(demo C CXX ASM)
# ===================设置工程目录======================
set(APP_DIR App/demo)
# ===================设置工程目录======================


# =====================设置项目文件=====================
set(MAP_FILE "${CMAKE_SOURCE_DIR}/Debug/demo.map")
set(LINKINFO_XML "${CMAKE_SOURCE_DIR}/Debug/demo_linkInfo.xml")
set(TARGET_FILE ${PROJECT_NAME}.out)


# =======添加编译标志=======
add_compile_options(-v5509A)
add_compile_definitions(c5509a;CHIP_5509A)
add_compile_options(--relaxed_ansi --gcc )
add_compile_options(--memory_model=small --ptrdiff_size=16)
add_compile_options(--display_error_number --diag_warning=225)
# 编译优化
add_compile_options(-O -g)
# =======添加链接标志=======
add_link_options(--display_error_number --diag_warning=225 -z -m${MAP_FILE} --stack_size=0x200 --heap_size=0x400)
add_link_options(--reread_libs --display_error_number --warn_sections)
add_link_options(--xml_link_info=${LINKINFO_XML} --rom_model --sys_stacksize=0x200)


## ====调试与预处理配置====
# 为Clangd提供compile_commands.json
#set(CMAKE_EXPORT_COMPILE_COMMANDS ON)  # 生成编译数据库
#set(CMAKE_VERBOSE_MAKEFILE ON)         # 显示详细编译命令

#-------------------------------------------库-------------------------------------------
file(GLOB_RECURSE SOURCES
        "${APP_DIR}/*.c"
        "BSP/src/*.c"
        "Core/src/*.c"
)
include_directories([[E:/Tools/Develop/ToolsKits/TI/C5500_Code_Generation_Tools4.4.1/include]]
        "App"
        "App/conf"
        "BSP/inc"
        "Core/Drivers/inc"
)

add_executable(${TARGET_FILE} ${SOURCES})
# 链接库
target_include_directories(${TARGET_FILE} PRIVATE
        "E:/Tools/Develop/ToolsKits/TI/C5500_Code_Generation_Tools4.4.1/include"
)
target_link_directories(${TARGET_FILE} PRIVATE
        "E:/Tools/Develop/ToolsKits/TI/C5500_Code_Generation_Tools4.4.1/lib"
)

# 添加自定义命令：
#add_custom_command(TARGET ${TARGET_FILE} POST_BUILD
#        COMMENT "Building "
#)