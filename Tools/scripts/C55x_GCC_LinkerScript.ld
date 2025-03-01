ENTRY(_start)            /* 指定入口点为启动文件中的_start符号 */

MEMORY {
    /* 程序存储器 (PMEM) - 通常为片上RAM或外部存储器 */
    PMEM (rx)   : ORIGIN = 0x00010000, LENGTH = 64K  /* 假设程序存储器起始地址 */

    /* 数据存储器 (DMEM) - 分多个物理bank */
    DARAM (rwx) : ORIGIN = 0x00020000, LENGTH = 64K  /* 双访问RAM */
    SARAM (rwx) : ORIGIN = 0x00030000, LENGTH = 192K  /* 单访问RAM */

    /* 外设寄存器空间 (需根据实际映射调整) */
    IO_REGS (rw) : ORIGIN = 0x00C00000, LENGTH = 1M
}

SECTIONS {
    /* 中断向量表 (需确认C55x向量表位置) */
    .intvecs : {
        KEEP(*(.intvecs))  /* 保持向量表不被优化 */
    } > PMEM

    /* 可执行代码段 */
    .text : {
        *(.text)           /* 主程序代码 */
        *(.text.*)         /* 函数级链接时可能需要 */
        KEEP(*(.init))     /* 系统初始化代码 */
        KEEP(*(.fini))     /* 系统终止代码 */
    } > SARAM

    /* 常量数据 (ROM中) */
    .rodata : {
        *(.rodata)
        *(.rodata.*)
    } > DARAM

    /* 初始化数据 (加载在PMEM，运行时拷贝到DARAM) */
    .data : AT (ADDR(.rodata) + SIZEOF(.rodata) + SIZEOF(.ARM.exidx)) {
        _data_load = LOADADDR(.data);  /* 记录加载地址 */
        _data_start = .;
        *(.data)
        *(.data.*)
        _data_end = .;
    } > DARAM

    /* 未初始化数据 (BSS段) */
    .bss (NOLOAD) : {
        __bss_start__ = .;
        *(.bss)
        *(.bss.*)
        *(COMMON)
        __bss_end__ = .;
    } > DARAM

    /* 堆空间 (通常放在剩余DARAM) */
    .heap (NOLOAD) : {
        _heap_start = .;
        . += 0x0400;       /* 最小4KB堆 */
        _heap_end = .;
    } > DARAM

    /* 堆栈 (通常使用SARAM) */
    .stack (NOLOAD) : {
        _stack_top = .;
        . += 0x0200;       /* 2KB系统堆栈 */
        _stack = .;
    } > DARAM



    /* 特殊功能寄存器 */
    .io_regs : {
        KEEP(*(.io_regs))  /* 如果代码中使用寄存器段声明 */
    } > IO_REGS

    /* 调试信息 (不影响执行) */
    .debug_info     0 : { *(.debug_info) }
    .debug_line     0 : { *(.debug_line) }
    .debug_abbrev   0 : { *(.debug_abbrev) }
    .debug_frame    0 : { *(.debug_frame) }
}

/* 符号导出供启动代码使用 */
PROVIDE(__stack = _stack);
PROVIDE(_system_stack_size = 0x0200);