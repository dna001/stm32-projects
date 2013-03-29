/*
    Default linker script for STM32F4xx_1024K_192K with GNU-ld
*/

/* Program Entry, set to mark it as "used" and avoid gc */
ENTRY(Reset_Handler)

/* Memory Spaces Definitions */
MEMORY
{
    FLASH   (RX)  : org = 0x08000000, len = 1M
    RAM     (RWX) : org = 0x20000000, len = 112k
    ETHRAM  (RWX) : org = 0x2001C000, len = 16k
    CCMRAM  (RWX) : org = 0x10000000, len = 64k
    EXTSRAM (RWX) : org = 0x68000000, len = 0
}

/* higher address of the user mode stack */
_estack = ORIGIN(RAM)+LENGTH(RAM);

/* There will be a link error if there is not this amount of RAM free at the end. */
_Minimum_Stack_Size = 0x100 ;

/* Sections Definitions */

SECTIONS
{
    . = 0;
    _text = .;

    startup : ALIGN(16) SUBALIGN(16)
    {
        KEEP(*(.isr_vector))
    } > FLASH

    /* the program code is stored in the .text section, which goes to Flash */
    .text : ALIGN(16) SUBALIGN(16)
    {
        *(.text.startup.*)
        *(.text)
        *(.text.*)
        *(.rodata)
        *(.rodata.*)
        *(.glue_7t)
        *(.glue_7)
        *(.gcc*)
    } > FLASH

    .ARM.extab :
    {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } > FLASH

    .ARM.exidx : {
        PROVIDE(__exidx_start = .);
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
        PROVIDE(__exidx_end = .);
        . = ALIGN(4);
     } > FLASH

    _etext = .;
    /* This is used by the startup in order to initialize the .data secion */
    _sidata = _etext;

    .data : AT (_sidata )
    {
        . = ALIGN(4);
        /* This is used by the startup in order to initialize the .data section */
        _sdata = .;
        *(.data)
        . = ALIGN(4);
        *(.data.*)
        . = ALIGN(4);
        *(.ramtext)
        . = ALIGN(4);
        /* This is used by the startup in order to initialize the .data section */
        _edata = .;
    } > RAM

    .bss (NOLOAD):
    {
        . = ALIGN(4);
        /* This is used by the startup in order to initialize the .bss section */
        _sbss = .;
        *(.bss)
        . = ALIGN(4);
        *(.bss.*)
        . = ALIGN(4);
        *(COMMON)
        . = ALIGN(4);
         /* This is used by the startup in order to initialize the .bss section */
         _ebss = . ;
    } > RAM

    PROVIDE ( end = _ebss );
    PROVIDE ( _end = _ebss );

    /* This is the user stack section 
    This is just to check that there is enough RAM left for the User mode stack
    It should generate an error if it's full.
     */
    ._usrstack (NOLOAD):
    {
        . = ALIGN(4);
        _susrstack = . ;
        . = . + _Minimum_Stack_Size ;
        . = ALIGN(4);
        _eusrstack = . ;
    } >RAM

    /* after that it's only debugging information. */
    
    /* remove the debugging information from the standard libraries */
/*    DISCARD :
    {
     libc.a ( * )
     libm.a ( * )
     libgcc.a ( * )
     }
*/

}

/* mthomas:  from the STM example - moved here for now to keep it as reference, not 
   used in the project (useful anyway?)*/

/* default stack size(s). These are used by the startup in order to allocate stacks for 
the different modes. */
__Stack_Size = 1024 ;

PROVIDE ( _Stack_Size = __Stack_Size ) ;
__Stack_Init = _estack - __Stack_Size ;

/*"PROVIDE" allows to easily override these values from an object file or the commmand line.*/
PROVIDE ( _Stack_Init = __Stack_Init ) ;
