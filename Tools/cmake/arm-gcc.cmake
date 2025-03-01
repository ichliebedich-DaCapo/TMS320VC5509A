# 设置编译工具集
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
set(CMAKE_AR arm-none-eabi-ar)
set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
set(CMAKE_OBJDUMP arm-none-eabi-objdump)
set(CMAKE_SIZE arm-none-eabi-size)

# 设置编译标准
set(CMAKE_CXX_STANDARD 98)
set(CMAKE_C_STANDARD 99)

#--------------------------编译选项----------------------------
add_compile_options(-mcpu=cortex-m4 -mthumb -mthumb-interwork)
add_compile_options(-mfloat-abi=hard -mfpu=fpv4-sp-d16)
add_compile_options(-ffunction-sections -fdata-sections -fno-common -fmessage-length=0)
# 允许预处理器处理汇编文件
add_compile_options($<$<COMPILE_LANGUAGE:ASM>:-x$<SEMICOLON>assembler-with-cpp>)


#--------------------------编译优化----------------------------
add_compile_options(-O2 -g)
add_compile_options(-finline-functions)
# 关闭异常处理
add_compile_options(-fno-exceptions)
# 关闭运行时错误处理(可以降低一些ROM占用)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")

# ----------------------调试选项--------------------------
# 开启内联警告，当函数内联失败时，编译器会发出警告。
add_compile_options(-Winline)

#---------------------------链接选项--------------------------
# 链接标志：指在编译和链接过程中，传递给链接器（Linker）的选项或参数，和链接选项是一个东西
add_link_options(-mfloat-abi=hard -mfpu=fpv4-sp-d16)
add_link_options(-mcpu=cortex-m4 -mthumb -mthumb-interwork)
add_link_options(-Wl,-gc-sections,--relax,--as-needed,--print-memory-usage,-Map=${CMAKE_BINARY_DIR}/${PROJECT_NAME}.map)
# 添加链接脚本
add_link_options(-T${CMAKE_SOURCE_DIR}/Tools/scripts/C55x_GCC_LinkerScript.ld)

# =====================设置项目文件=====================
set(TARGET_FILE ${PROJECT_NAME}.elf)
set(CMAKE_C_COMPILER_WORKS ON)   # 强制绕过编译器检查
set(CMAKE_CXX_COMPILER_WORKS ON)
#set(CMAKE_C_COMPILER_FORCED ON)