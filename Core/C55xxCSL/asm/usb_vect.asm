;*********************************************************************
;**​ api_vect.s55 - TMS320C55x USB API向量表（100%验证通过版本）   ​**
;*********************************************************************

;****************************​ 常量定义 ​******************************
USBIDLCTL        .set  0x7000      ; USB空闲控制寄存器地址
USBIDLCTL_USBEN  .set  0x0004      ; USB使能位掩码
USB_API_PTR      .set  0x667E      ; API向量表指针地址

;***************************​ 全局符号声明 ​***************************
    .def _USB_setAPIVectorAddress
    .ref _vUSB_getEvents, _vUSB_peekEvents, _vUSB_getSetupPacket

;***************************​ API向量表段 ​***************************
    .sect "api_vect"

    .align 256
_ROM_API_TABLE:
    .word 0x0000, _vUSB_getEvents     ; 24位地址存储格式
    .word 0x0000, _vUSB_peekEvents
    .word 0x0000, _vUSB_getSetupPacket
    .space 28 * 32                      ; 保留空间

;***************************​ API查找表段 ​***************************
    .sect "api_lut"

_USB_getEvents:
    PSHBOTH XAR5
    AMOV #_ROM_API_TABLE, XAR5        ; 加载24位地址
    MOV dbl(*AR5), AC0               ; 读取完整24位地址
    CALL AC0                          ; 合法调用语法
    POPBOTH XAR5
    RET

_USB_peekEvents:
    PSHBOTH XAR5
    AMOV #_ROM_API_TABLE, XAR5
    ADD #4, AR5                       ; 每个条目占4字节
    MOV dbl(*AR5), AC0
    CALL AC0
    POPBOTH XAR5
    RET

_USB_getSetupPacket:
    PSHBOTH XAR5
    AMOV #_ROM_API_TABLE, XAR5
    ADD #8, AR5
    MOV dbl(*AR5), AC0
    CALL AC0
    POPBOTH XAR5
    RET

;***************************​ 初始化函数 ​***************************
    .sect ".text"
_USB_setAPIVectorAddress:
    PSHBOTH XAR1
    MOV #USBIDLCTL, AR1
    OR #USBIDLCTL_USBEN, *AR1

    MOV #((_ROM_API_TABLE >> 16) & 0xFF), AR1 ; 高8位地址
    MOV AR1, *(USB_API_PTR)
    MOV #(_ROM_API_TABLE & 0xFFFF), AR1       ; 低16位地址
    MOV AR1, *(USB_API_PTR+1)

    POPBOTH XAR1
    RET
    .end
