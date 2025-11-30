;******************** (C) COPYRIGHT 2011 STMicroelectronics ********************
;* File Name          : startup_stm32f10x_md.s
;* Author             : MCD Application Team
;* Version            : V3.5.0
;* Date               : 11-March-2011
;* Description        : STM32F10x Medium Density Devices vector table for MDK-ARM 
;*                      toolchain.  
;*                      This module performs:
;*                      - Set the initial SP
;*                      - Set the initial PC == Reset_Handler
;*                      - Set the vector table entries with the exceptions ISR address
;*                      - Configure the clock system
;*                      - Branches to __main in the C library (which eventually
;*                        calls main()).
;*                      After Reset the CortexM3 processor is in Thread mode,
;*                      priority is Privileged, and the Stack is set to Main.
;* <<< Use Configuration Wizard in Context Menu >>>   


;******************** (C) COPYRIGHT 2011 STMicroelectronics ********************
;* 文件名           : startup_stm32f10x_md.s
;* 作者             : MCD应用团队
;* 版本             : V3.5.0
;* 日期             : 2011年3月11日
;* 描述            : 用于MDK-ARM工具链的STM32F10x中等容量设备向量表
;*                      本模块执行以下功能：
;*                      - 设置初始栈指针(SP)
;*                      - 设置初始程序计数器(PC) == Reset_Handler,  设置程序入口地址?
;*                      - 设置向量表条目与异常ISR地址
;*                      - 配置时钟系统
;*                      - 跳转到C库中的__main（最终调用main()函数）
;*                      复位后，Cortex-M3处理器处于线程模式，
;*                      优先级为特权级，栈设置为主栈
;*  在上下文菜单中使用配置向导    


;*******************************************************************************
; THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
; WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
; AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
; INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
; CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
; INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
;*******************************************************************************

;*******************************************************************************
; 本固件仅用于指导目的，旨在为客户提供有关其产品的编码信息以节省时间。
; 因此，对于因使用此类固件内容和/或客户使用其中包含的编码信息而导致的
; 任何直接、间接或附带损失，STMICROELECTRONICS 概不负责。
;*******************************************************************************


; Amount of memory (in bytes) allocated for Stack
; Tailor this value to your application needs
; <h> Stack Configuration
;   <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>


; 为栈分配的内存大小（字节）
; 根据您的应用程序需求调整此值
; <h> 栈配置
;   <o> 栈大小（字节） <0x0-0xFFFFFFFF:8>
; </h>



Stack_Size      EQU     0x00000400  ;定义栈大小为1024字节 (0x400 = 1024)

                AREA    STACK, NOINIT, READWRITE, ALIGN=3  ;定义栈区域，不初始化，可读写，8字节对齐
Stack_Mem       SPACE   Stack_Size	; 为栈分配指定大小的空间
__initial_sp  	; 栈顶指针标签，系统启动时使用


;*******************************************************************************
; <h> Heap Configuration
;   <o>  Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

; 为堆分配的内存大小（字节）
; 根据您的应用程序需求调整此值
; <h> 堆配置
;   <o> 堆大小（字节） <0x0-0xFFFFFFFF:8>
; </h>
 


Heap_Size       EQU     0x00000200	; 定义堆大小为512字节 (0x200 = 512)

                AREA    HEAP, NOINIT, READWRITE, ALIGN=3 	; 定义堆区域，不初始化，可读写，8字节对齐
__heap_base					; 堆起始地址标签
Heap_Mem        SPACE   Heap_Size	; 为堆分配指定大小的空间
__heap_limit				; 堆结束地址标签		

                PRESERVE8	; 保持8字节对齐
                THUMB		; 使用Thumb指令集


;当系统复位或上电时，Cortex-M3 内核硬件会自动执行以下操作：
;	地址 0x00000000‌：硬件从该地址读取第一个字作为初始主栈指针(MSP)值
;	地址 0x00000004‌：硬件从该地址读取第二个字作为复位向量地址，跳转到 Reset_Handler

; Vector Table Mapped to Address 0 at Reset		; 复位时映射到地址0的向量表
                AREA    RESET, DATA, READONLY	; 定义只读的复位向量表区域
                EXPORT  __Vectors				 ; 导出向量表起始地址，向里表里声明了对应的中断方法名(使用弱符号),在 C 文件里实现就可以了.
                EXPORT  __Vectors_End			 ; 导出向量表结束地址
                EXPORT  __Vectors_Size			 ; 导出向量表大小	

 
; ******************** STM32F103C8T6 中断向量表 ********************
; 向量表起始标签，定义 在Flash 存储器的起始位置
__Vectors       DCD     __initial_sp               ; 地址0x00000000: 初始栈顶指针
                DCD     Reset_Handler              ; 地址0x00000004: 复位处理程序
                DCD     NMI_Handler                ; 地址0x00000008: 不可屏蔽中断处理程序
                DCD     HardFault_Handler          ; 地址0x0000000C: 硬件错误处理程序
                DCD     MemManage_Handler          ; 地址0x00000010: 存储器保护单元错误处理程序
                DCD     BusFault_Handler           ; 地址0x00000014: 总线错误处理程序
                DCD     UsageFault_Handler         ; 地址0x00000018: 用法错误处理程序
                DCD     0                          ; 地址0x0000001C: 保留
                DCD     0                          ; 地址0x00000020: 保留
                DCD     0                          ; 地址0x00000024: 保留
                DCD     0                          ; 地址0x00000028: 保留
                DCD     SVC_Handler                ; 地址0x0000002C: 系统服务调用处理程序
                DCD     DebugMon_Handler           ; 地址0x00000030: 调试监控处理程序
                DCD     0                          ; 地址0x00000034: 保留
                DCD     PendSV_Handler             ; 地址0x00000038: 可挂起的系统调用处理程序
                DCD     SysTick_Handler            ; 地址0x0000003C: 系统节拍定时器处理程序

                ; ********** 外部中断向量表 **********
                DCD     WWDG_IRQHandler            ; 中断号0: 窗口看门狗中断
                DCD     PVD_IRQHandler             ; 中断号1: 电源电压检测通过EXTI线路检测
                DCD     TAMPER_IRQHandler          ; 中断号2: 篡改检测中断
                DCD     RTC_IRQHandler             ; 中断号3: 实时时钟中断
                DCD     FLASH_IRQHandler           ; 中断号4: Flash存储器中断
                DCD     RCC_IRQHandler             ; 中断号5: 复位和时钟控制器中断
                DCD     EXTI0_IRQHandler           ; 中断号6: 外部中断线0
                DCD     EXTI1_IRQHandler           ; 中断号7: 外部中断线1
                DCD     EXTI2_IRQHandler           ; 中断号8: 外部中断线2
                DCD     EXTI3_IRQHandler           ; 中断号9: 外部中断线3
                DCD     EXTI4_IRQHandler           ; 中断号10: 外部中断线4
                DCD     DMA1_Channel1_IRQHandler   ; 中断号11: DMA1通道1中断
                DCD     DMA1_Channel2_IRQHandler   ; 中断号12: DMA1通道2中断
                DCD     DMA1_Channel3_IRQHandler   ; 中断号13: DMA1通道3中断
                DCD     DMA1_Channel4_IRQHandler   ; 中断号14: DMA1通道4中断
                DCD     DMA1_Channel5_IRQHandler   ; 中断号15: DMA1通道5中断
                DCD     DMA1_Channel6_IRQHandler   ; 中断号16: DMA1通道6中断
                DCD     DMA1_Channel7_IRQHandler   ; 中断号17: DMA1通道7中断
                DCD     ADC1_2_IRQHandler          ; 中断号18: ADC1和ADC2中断
                DCD     USB_HP_CAN1_TX_IRQHandler  ; 中断号19: USB高优先级或CAN1发送中断
                DCD     USB_LP_CAN1_RX0_IRQHandler ; 中断号20: USB低优先级或CAN1接收0中断
                DCD     CAN1_RX1_IRQHandler        ; 中断号21: CAN1接收1中断
                DCD     CAN1_SCE_IRQHandler        ; 中断号22: CAN1 SCE中断
                DCD     EXTI9_5_IRQHandler         ; 中断号23: 外部中断线9到5
                DCD     TIM1_BRK_IRQHandler        ; 中断号24: 定时器1刹车中断
                DCD     TIM1_UP_IRQHandler         ; 中断号25: 定时器1更新中断
                DCD     TIM1_TRG_COM_IRQHandler    ; 中断号26: 定时器1触发和通信中断
                DCD     TIM1_CC_IRQHandler         ; 中断号27: 定时器1捕获比较中断
                DCD     TIM2_IRQHandler            ; 中断号28: 定时器2全局中断
                DCD     TIM3_IRQHandler            ; 中断号29: 定时器3全局中断
                DCD     TIM4_IRQHandler            ; 中断号30: 定时器4全局中断
                DCD     I2C1_EV_IRQHandler         ; 中断号31: I2C1事件中断
                DCD     I2C1_ER_IRQHandler         ; 中断号32: I2C1错误中断
                DCD     I2C2_EV_IRQHandler         ; 中断号33: I2C2事件中断
                DCD     I2C2_ER_IRQHandler         ; 中断号34: I2C2错误中断
                DCD     SPI1_IRQHandler            ; 中断号35: SPI1中断
                DCD     SPI2_IRQHandler            ; 中断号36: SPI2中断
                DCD     USART1_IRQHandler          ; 中断号37: USART1全局中断
                DCD     USART2_IRQHandler          ; 中断号38: USART2全局中断
                DCD     USART3_IRQHandler          ; 中断号39: USART3全局中断
                DCD     EXTI15_10_IRQHandler       ; 中断号40: 外部中断线15到10
                DCD     RTCAlarm_IRQHandler        ; 中断号41: 通过EXTI线路的RTC闹钟中断
                DCD     USBWakeUp_IRQHandler       ; 中断号42: USB从挂起状态唤醒中断
__Vectors_End ; 向量表结束标签

; 计算向量表的总大小（以字节为单位）
; 公式：向量表结束地址 - 向量表起始地址
__Vectors_Size  EQU  __Vectors_End - __Vectors	;通过EQU指令定义向量表的总字节大小，用于系统初始化时配置NVIC



; 定义代码段区域
; |.text| - 代码段名称
; CODE - 表示代码段
; READONLY - 只读属性
                AREA    |.text|, CODE, READONLY

; 复位处理程序定义
Reset_Handler    PROC                          ; 开始复位处理程序过程
                 EXPORT  Reset_Handler             [WEAK]  ; 弱导出复位处理程序，允许用户重写
     IMPORT  __main                            ; 声明外部符号__main（C库初始化函数）
     IMPORT  SystemInit                       ; 声明外部符号SystemInit（系统初始化函数）
                 LDR     R0, =SystemInit        ; 将SystemInit函数地址加载到寄存器R0
                 BLX     R0                     ; 带链接的跳转，调用SystemInit函数
                 LDR     R0, =__main            ; 将__main函数地址加载到寄存器R0
                 BX      R0                     ; 跳转到__main函数
                 ENDP                           ; 结束复位处理程序过程




;使用弱符号（WEAK）定义所有中断处理程序，允许用户在应用程序中重写实现
;所有默认处理程序都是无限循环，便于调试时识别未处理的中断
;包含16个内核异常和43个外设中断的默认实现
;采用汇编语言编写，直接与硬件交互
;为每个中断提供独立的处理过程，确保系统稳定运行 

; Dummy Exception Handlers (infinite loops which can be modified)
; 虚拟异常处理程序（可修改的无限循环，用于调试和错误处理）

; 不可屏蔽中断处理程序
NMI_Handler     PROC                          ; 定义NMI处理程序过程
                EXPORT  NMI_Handler                [WEAK]  ; 弱导出，允许用户重写实现
                B       .                       ; 无限循环（跳转到当前地址）
                ENDP                           ; 结束过程定义

; 硬件错误处理程序（反斜杠表示续行）
HardFault_Handler\
                PROC                          ; 定义硬件错误处理程序过程
                EXPORT  HardFault_Handler          [WEAK]  ; 弱导出硬件错误处理程序
                B       .                       ; 无限循环
                ENDP                           ; 结束过程定义

; 存储器管理单元错误处理程序
MemManage_Handler\
                PROC                          ; 定义存储器管理错误处理程序
                EXPORT  MemManage_Handler          [WEAK]  ; 弱导出
                B       .                       ; 无限循环
                ENDP                           ; 结束过程定义

; 总线错误处理程序
BusFault_Handler\
                PROC                          ; 定义总线错误处理程序
                EXPORT  BusFault_Handler           [WEAK]  ; 弱导出
                B       .                       ; 无限循环
                ENDP                           ; 结束过程定义

; 用法错误处理程序
UsageFault_Handler\
                PROC                          ; 定义用法错误处理程序
                EXPORT  UsageFault_Handler         [WEAK]  ; 弱导出
                B       .                       ; 无限循环
                ENDP                           ; 结束过程定义

; 系统服务调用处理程序
SVC_Handler     PROC                          ; 定义系统服务调用处理程序
                EXPORT  SVC_Handler                [WEAK]  ; 弱导出
                B       .                       ; 无限循环
                ENDP                           ; 结束过程定义

; 调试监控处理程序
DebugMon_Handler\
                PROC                          ; 定义调试监控处理程序
                EXPORT  DebugMon_Handler           [WEAK]  ; 弱导出
                B       .                       ; 无限循环
                ENDP                           ; 结束过程定义

; 可挂起的系统调用处理程序
PendSV_Handler  PROC                          ; 定义可挂起系统调用处理程序
                EXPORT  PendSV_Handler             [WEAK]  ; 弱导出
                B       .                       ; 无限循环
                ENDP                           ; 结束过程定义

; 系统节拍定时器处理程序
SysTick_Handler PROC                          ; 定义系统节拍定时器处理程序
                EXPORT  SysTick_Handler            [WEAK]  ; 弱导出
                B       .                       ; 无限循环
                ENDP                           ; 结束过程定义

; 默认中断处理程序
Default_Handler PROC                          ; 定义默认中断处理程序过程

                ; 导出所有外设中断处理程序（弱符号）
                EXPORT  WWDG_IRQHandler            [WEAK]  ; 窗口看门狗中断
                EXPORT  PVD_IRQHandler             [WEAK]  ; 电源电压检测中断
                EXPORT  TAMPER_IRQHandler          [WEAK]  ; 篡改检测中断
                EXPORT  RTC_IRQHandler             [WEAK]  ; 实时时钟中断
                EXPORT  FLASH_IRQHandler           [WEAK]  ; Flash存储器中断
                EXPORT  RCC_IRQHandler             [WEAK]  ; 复位和时钟控制器中断
                EXPORT  EXTI0_IRQHandler           [WEAK]  ; 外部中断线0
                EXPORT  EXTI1_IRQHandler           [WEAK]  ; 外部中断线1
                EXPORT  EXTI2_IRQHandler           [WEAK]  ; 外部中断线2
                EXPORT  EXTI3_IRQHandler           [WEAK]  ; 外部中断线3
                EXPORT  EXTI4_IRQHandler           [WEAK]  ; 外部中断线4
                EXPORT  DMA1_Channel1_IRQHandler   [WEAK]  ; DMA1通道1中断
                EXPORT  DMA1_Channel2_IRQHandler   [WEAK]  ; DMA1通道2中断
                EXPORT  DMA1_Channel3_IRQHandler   [WEAK]  ; DMA1通道3中断
                EXPORT  DMA1_Channel4_IRQHandler   [WEAK]  ; DMA1通道4中断
                EXPORT  DMA1_Channel5_IRQHandler   [WEAK]  ; DMA1通道5中断
                EXPORT  DMA1_Channel6_IRQHandler   [WEAK]  ; DMA1通道6中断
                EXPORT  DMA1_Channel7_IRQHandler   [WEAK]  ; DMA1通道7中断
                EXPORT  ADC1_2_IRQHandler          [WEAK]  ; ADC1和ADC2中断
                EXPORT  USB_HP_CAN1_TX_IRQHandler  [WEAK]  ; USB高优先级或CAN1发送中断
                EXPORT  USB_LP_CAN1_RX0_IRQHandler [WEAK]  ; USB低优先级或CAN1接收0中断
                EXPORT  CAN1_RX1_IRQHandler        [WEAK]  ; CAN1接收1中断
                EXPORT  CAN1_SCE_IRQHandler        [WEAK]  ; CAN1 SCE中断
                EXPORT  EXTI9_5_IRQHandler         [WEAK]  ; 外部中断线9到5
                EXPORT  TIM1_BRK_IRQHandler        [WEAK]  ; 定时器1刹车中断
                EXPORT  TIM1_UP_IRQHandler         [WEAK]  ; 定时器1更新中断
                EXPORT  TIM1_TRG_COM_IRQHandler    [WEAK]  ; 定时器1触发和通信中断
                EXPORT  TIM1_CC_IRQHandler         [WEAK]  ; 定时器1捕获比较中断
                EXPORT  TIM2_IRQHandler            [WEAK]  ; 定时器2全局中断
                EXPORT  TIM3_IRQHandler            [WEAK]  ; 定时器3全局中断
                EXPORT  TIM4_IRQHandler            [WEAK]  ; 定时器4全局中断
                EXPORT  I2C1_EV_IRQHandler         [WEAK]  ; I2C1事件中断
                EXPORT  I2C1_ER_IRQHandler         [WEAK]  ; I2C1错误中断
                EXPORT  I2C2_EV_IRQHandler         [WEAK]  ; I2C2事件中断
                EXPORT  I2C2_ER_IRQHandler         [WEAK]  ; I2C2错误中断
                EXPORT  SPI1_IRQHandler            [WEAK]  ; SPI1中断
                EXPORT  SPI2_IRQHandler            [WEAK]  ; SPI2中断
                EXPORT  USART1_IRQHandler          [WEAK]  ; USART1全局中断
                EXPORT  USART2_IRQHandler          [WEAK]  ; USART2全局中断
                EXPORT  USART3_IRQHandler          [WEAK]  ; USART3全局中断
                EXPORT  EXTI15_10_IRQHandler       [WEAK]  ; 外部中断线15到10
                EXPORT  RTCAlarm_IRQHandler        [WEAK]  ; 通过EXTI线路的RTC闹钟中断
                EXPORT  USBWakeUp_IRQHandler       [WEAK]  ; USB从挂起状态唤醒中断

; 所有外设中断处理程序的实际实现标签
WWDG_IRQHandler                            ; 窗口看门狗中断标签
PVD_IRQHandler                             ; 电源电压检测中断标签
TAMPER_IRQHandler                          ; 篡改检测中断标签
RTC_IRQHandler                             ; 实时时钟中断标签
FLASH_IRQHandler                           ; Flash存储器中断标签
RCC_IRQHandler                             ; 复位和时钟控制器中断标签
EXTI0_IRQHandler                           ; 外部中断线0标签
EXTI1_IRQHandler                           ; 外部中断线1标签
EXTI2_IRQHandler                           ; 外部中断线2标签
EXTI3_IRQHandler                           ; 外部中断线3标签
EXTI4_IRQHandler                           ; 外部中断线4标签
DMA1_Channel1_IRQHandler                   ; DMA1通道1中断标签
DMA1_Channel2_IRQHandler                   ; DMA1通道2中断标签
DMA1_Channel3_IRQHandler                   ; DMA1通道3中断标签
DMA1_Channel4_IRQHandler                   ; DMA1通道4中断标签
DMA1_Channel5_IRQHandler                   ; DMA1通道5中断标签
DMA1_Channel6_IRQHandler                   ; DMA1通道6中断标签
DMA1_Channel7_IRQHandler                   ; DMA1通道7中断标签
ADC1_2_IRQHandler                          ; ADC1和ADC2中断标签
USB_HP_CAN1_TX_IRQHandler                  ; USB高优先级或CAN1发送中断标签
USB_LP_CAN1_RX0_IRQHandler                 ; USB低优先级或CAN1接收0中断标签
CAN1_RX1_IRQHandler                        ; CAN1接收1中断标签
CAN1_SCE_IRQHandler                        ; CAN1 SCE中断标签
EXTI9_5_IRQHandler                         ; 外部中断线9到5标签
TIM1_BRK_IRQHandler                         ; 定时器1刹车中断标签
TIM1_UP_IRQHandler                          ; 定时器1更新中断标签
TIM1_TRG_COM_IRQHandler                      ; 定时器1触发和通信中断标签
TIM1_CC_IRQHandler                          ; 定时器1捕获比较中断标签
TIM2_IRQHandler                             ; 定时器2全局中断标签
TIM3_IRQHandler                             ; 定时器3全局中断标签
TIM4_IRQHandler                             ; 定时器4全局中断标签
I2C1_EV_IRQHandler                          ; I2C1事件中断标签
I2C1_ER_IRQHandler                          ; I2C1错误中断标签
I2C2_EV_IRQHandler                          ; I2C2事件中断标签
I2C2_ER_IRQHandler                          ; I2C2错误中断标签
SPI1_IRQHandler                             ; SPI1中断标签
SPI2_IRQHandler                             ; SPI2中断标签
USART1_IRQHandler                           ; USART1全局中断标签
USART2_IRQHandler                           ; USART2全局中断标签
USART3_IRQHandler                           ; USART3全局中断标签
EXTI15_10_IRQHandler                        ; 外部中断线15到10标签
RTCAlarm_IRQHandler                         ; 通过EXTI线路的RTC闹钟中断标签
USBWakeUp_IRQHandler                         ; USB从挂起状态唤醒中断标签

                B       .                       ; 默认无限循环（所有未实现的中断都会跳转到这里）

                ENDP                           ; 结束默认处理程序过程

                ALIGN                          ; 确保代码按字对齐



;*******************************************************************************
; 用户栈和堆的初始化
;条件编译机制‌：根据是否使用微库（MICROLIB）选择不同的内存管理策略
;微库模式‌：直接导出栈和堆的边界符号，由微库负责内存管理
;标准库模式‌：实现__user_initial_stackheap函数，返回栈和堆的配置参数
;内存布局配置‌：
;	R0 = 堆起始地址
;	R1 = 栈结束地址（栈底）
;	R2 = 堆结束地址
;	R3 = 栈起始地址（栈顶）
;系统兼容性‌：确保与不同C库的内存管理机制兼容
;函数返回值‌：通过BX LR指令返回调用者，完成栈堆初始化

;*******************************************************************************
                 IF      :DEF:__MICROLIB           ; 如果定义了__MICROLIB宏（使用微库）
                
                 EXPORT  __initial_sp                ; 导出初始栈指针地址
                 EXPORT  __heap_base                 ; 导出堆起始地址
                 EXPORT  __heap_limit                 ; 导出堆结束地址
                
                 ELSE                                ; 否则（使用标准C库）
                
                 IMPORT  __use_two_region_memory     ; 导入双区域内存管理符号
                 EXPORT  __user_initial_stackheap    ; 导出用户初始栈堆设置函数
                 
__user_initial_stackheap                      ; 用户初始栈堆设置函数标签

                 LDR     R0, =  Heap_Mem           ; 将堆起始地址加载到寄存器R0
                 LDR     R1, =(Stack_Mem + Stack_Size) ; 将栈结束地址（栈底）加载到R1
                 LDR     R2, = (Heap_Mem +  Heap_Size) ; 将堆结束地址加载到R2
                 LDR     R3, = Stack_Mem           ; 将栈起始地址加载到R3
                 BX      LR                       ; 返回调用者（链接寄存器）

                 ALIGN                            ; 确保代码按字对齐

                 ENDIF                             ; 结束条件编译块

                 END                               ; 文件结束标记

;******************* (C) COPYRIGHT 2011 STMicroelectronics *****文件结束*****

