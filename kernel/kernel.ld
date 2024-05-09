OUTPUT_ARCH( "riscv" )
ENTRY( _entry )

SECTIONS
{
    . = 0x80000000;

    .text :{
        *(.text)
         . = ALIGN(0x1000);
         *(trampsec)
    }
    etext = .;


    .rodata :{
        *(.rodata)
    }

    .data :{
        *(.data)
    }

    .bss :{
        *(.bss)
    }

    PROVIDE(end = .);
}