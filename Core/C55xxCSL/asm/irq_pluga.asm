;\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
; CSL中断向量安装函数二次修正版
; 主要修正点：
; 1. 修正PSH/POP指令语法
; 2. 符合CPL=1模式的堆栈操作
; 3. 优化条件判断逻辑
;\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

    .asg 123, CSL_DATA_PTR
    .asg 05E80h, NOP16_Operator
    .asg 0, VecLoc
    .asg 2, IsrAddr
    .asg 4, Iptr
    .asg 5, EventBit
    .asg 6, EventMask
    .asg 7, EventId
    .asg 8, Old_INTM
    .asg 9, LocalFrameSz
    .asg 12, BiosPresentFlag
    .asg 049h, IVPD_ADDR
    .asg 04Ah, IVPH_ADDR
    .asg 001h, IFR0_ADDR
    .asg 046h, IFR1_ADDR

    .mmregs
    .def _IRQ_plug
    .ref _IRQ_globalDisable, _IRQ_globalRestore

    .cpl_on        ; 设置CPL模式

_IRQ_plug:
    PSHBOTH XAR3
    PSHBOTH XAR2
    AADD #-LocalFrameSz, SP

    MOV AC0, dbl(*SP(#IsrAddr))
    MOV T0, *SP(#EventId)

    MOV #0, AC0
    MOV AC0, XAR2
    MOV AC0, XAR3

    CALL _IRQ_globalDisable
    MOV T0, *SP(#Old_INTM)

    MOV #IVPD_ADDR, AR2          ; 使用MOV加载地址到AR2
    MOV *SP(#EventId), AC1

    AND #0x18, AC1, AC3
    BTST @#4, AC3, TC1           ; 使用BTST测试第4位（0x10）
    BCC L_IVPH, TC1
    MOV #IVPH_ADDR, AR2          ; 使用MOV加载IVPH地址

L_IVPH:
    SFTS AC1, #3, AC1
    MOV *AR2, AC0
    SFTS AC0, #8, AC0
    OR AC1, AC0
    SFTS AC0, #-1
    MOV AC0, dbl(*SP(#VecLoc))

    MOV dbl(*SP(#IsrAddr)), AC0
    MOV dbl(*SP(#VecLoc)), XAR3  ; 使用MOV加载扩展地址

    MOV *SP(#EventId), AR1
    AND #0x0F, AR1, AR2
    MOV AC0, dbl(*AR3)

    MOV #NOP16_Operator, AC3
    ADD #2, AR3
    MOV AC3, *AR3+
    MOV AC3, *AR3+

    MOV AR2, T0
    AND #0x10, AR1
    MOV #1, AC0
    SFTS AC0, T0, AC0

    BTST @#4, AR1, TC1           ; 检测bit4
    ; 修正为C55x标准条件码
    BCC L_IFR0, !TC1  ; 使用逻辑非操作符+TC状态位
    MOV #IFR1_ADDR, AR2
    MOV AC0, *AR2
    B RESTORE_INTM

L_IFR0:
    MOV #IFR0_ADDR, AR2
    MOV AC0, *AR2

RESTORE_INTM:
    MOV *SP(#Old_INTM), T0
    CALL _IRQ_globalRestore

EPILOGUE:
    AADD #LocalFrameSz, SP
    POPBOTH XAR2
    POPBOTH XAR3
    RET
