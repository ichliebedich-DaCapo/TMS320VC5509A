# 指定CGT
#set(CMAKE_C_COMPILER_WORKS 1)   # 强制绕过编译器检查
# 强制禁用响应文件
#set(CMAKE_C_USE_RESPONSE_FILE_FOR_OBJECTS OFF)
#set(CMAKE_CXX_USE_RESPONSE_FILE_FOR_OBJECTS OFF)
set(CMAKE_C_COMPILER [[E:\Tools\Develop\ToolsKits\TI\C5500_Code_Generation_Tools4.4.1\bin\cl55.exe]])  # TI C55x编译器路径
set(CMAKE_CXX_COMPILER ${CMAKE_C_COMPILER})
set(CMAKE_ASM_COMPILER ${CMAKE_C_COMPILER})

# 链接文件
set(MAP_FILE "${CMAKE_SOURCE_DIR}/Debug/demo.map")
set(LINKINFO_XML "${CMAKE_SOURCE_DIR}/Debug/demo_linkInfo.xml")

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


