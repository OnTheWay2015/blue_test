
/* 
 * STM32F103C8 Linker Script
 * FLASH: 64KB (0x08000000 - 0x0800FFFF)
 * RAM:   20KB (0x20000000 - 0x20004FFF)
 */

MEMORY
{
  FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 64K
  RAM (rwx)  : ORIGIN = 0x20000000, LENGTH = 20K
}

/* 入口点 */
ENTRY(Reset_Handler)

/* 堆栈大小配置 */
_Min_Heap_Size = 0x200;  /* 512字节最小堆 */
_Min_Stack_Size = 0x400; /* 1KB最小栈 */

SECTIONS
{
  /* 中断向量表 */
  .isr_vector :
  {
    . = ALIGN(4);
    KEEP(*(.isr_vector))
    . = ALIGN(4);
  } >FLASH

  /* 程序代码和常量 */
  .text :
  {
    . = ALIGN(4);
    *(.text)
    *(.text*)
    *(.glue_7)
    *(.glue_7t)
    *(.eh_frame)

    KEEP (*(.init))
    KEEP (*(.fini))

    . = ALIGN(4);
    _etext = .;
  } >FLASH

  /* 只读数据 */
  .rodata :
  {
    . = ALIGN(4);
    *(.rodata)
    *(.rodata*)
    . = ALIGN(4);
  } >FLASH

  /* 初始化数据（从FLASH加载到RAM） */
  .data : AT (_etext)
  {
    . = ALIGN(4);
    _sdata = .;
    *(.data)
    *(.data*)
    . = ALIGN(4);
    _edata = .;
  } >RAM

  /* 未初始化数据（BSS段） */
  .bss :
  {
    . = ALIGN(4);
    _sbss = .;
    *(.bss)
    *(.bss*)
    *(COMMON)
    . = ALIGN(4);
    _ebss = .;
  } >RAM

  /* 用户堆空间 */
  .heap (NOLOAD) :
  {
    . = ALIGN(8);
    _heap_start = .;
    . = . + _Min_Heap_Size;
    . = ALIGN(8);
    _heap_end = .;
  } >RAM

  /* 主栈空间 */
  .stack (NOLOAD) :
  {
    . = ALIGN(8);
    _stack_end = .;
    . = . + _Min_Stack_Size;
    . = ALIGN(8);
    _stack_top = .;
  } >RAM

  /* 移除调试信息 */
  /DISCARD/ :
  {
    libc.a ( * )
    libm.a ( * )
    libgcc.a ( * )
  }

  /* 符号定义供代码使用 */
  _estack = ORIGIN(RAM) + LENGTH(RAM);
}
