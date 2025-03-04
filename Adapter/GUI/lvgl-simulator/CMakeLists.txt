cmake_minimum_required(VERSION 3.10)
project(Simulator LANGUAGES C CXX)
include(common_functions.cmake)

# ------------------------ 配置选项 ------------------------
set(PROJECT_DIR ../../../App/9_LCD)  # 项目选择
#options_enable_static_lib_ld()
# 引入命名空间后，这该死的静态库链接容易出问题
#message(WARNING "static_lib_ld has some problems!")
# ------------------------ 路径配置 ------------------------
# 基础路径
set(PROJECT_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/..")
set(THIRD_PARTY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../../Middleware/Third_Party")

# 组件路径配置
set(SDL2_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/SDL2/i686-w64-mingw32")
set(U8G2_DIR "${THIRD_PARTY_DIR}/u8g2")
set(UI_DIR "${PROJECT_DIR}/ui")  # 通过PROJECT_DIR自动生成
set(GUI_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../")

# 输出目录统一配置
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

# ------------------------ 编译选项 ------------------------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 90)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-O2 -g)
string(APPEND CMAKE_CXX_FLAGS " -fconcepts")  # 修正flags设置方式

# ------------------------ 依赖配置 ------------------------
# SDL2配置
find_library(SDL2_LIB SDL2 HINTS "${SDL2_ROOT}/lib" REQUIRED)

# ------------------------ 组件库定义 ------------------------
# U8G2库
set(U8G2_INC_DIRS "${U8G2_DIR}")
find_sources(U8G2_SRCS "${U8G2_DIR}")

# Drivers库
set(DRIVERS_INC_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/Drivers")
find_sources(DRIVERS_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/Drivers")

# GUI库
set(GUI_INC_DIRS "${GUI_DIR}")
file(GLOB_RECURSE GUI_SRCS "../gui.c")

# UI
set(UI_INC_DIRS "${UI_DIR}")
find_sources(UI_SRCS "${UI_DIR}")

# GUI库（聚合组件）
# ------------------------ 可执行目标 ------------------------
# 添加一个模拟器宏
add_compile_definitions(SIMULATOR)
add_compile_options(-Winline)
add_link_options(-Wl,-gc-sections,--relax,--as-needed,-Map=${CMAKE_BINARY_DIR}/${PROJECT_NAME}.map)
add_executable(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp" "test.cpp")

# 直接编译模式自动继承所有依赖
target_sources(${PROJECT_NAME} PRIVATE
        ${DRIVERS_SRCS}
        ${U8G2_SRCS}
        ${GUI_SRCS}
        ${UI_SRCS}
)
include_directories(
        ${DRIVERS_INC_DIRS}
        ${U8G2_INC_DIRS}
        ${GUI_INC_DIRS}
        ${SDL2_ROOT}/include
        ${UI_INC_DIRS}
)
target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL2_LIB})


# ------------------------ 后期处理 ------------------------
# 自动复制SDL2.dll
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        "${SDL2_ROOT}/bin/SDL2.dll"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
)

# 工具链提示
if (CMAKE_TOOLCHAIN_FILE)
    message(STATUS "Using toolchain file: ${CMAKE_TOOLCHAIN_FILE}")
endif ()

# 因为32位编译比64位快，所以没有使用64位编译工具链