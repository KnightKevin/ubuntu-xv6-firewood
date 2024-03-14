OUTPUT_ARCH( "riscv" )
ENTRY( _entry )

SECTIONS
{
    . = 0x80000000;

    .text :{
        *(.text)
    }

    .data :{
        *(.data)
    }

    .rodata :{
        *(.rodata)
    }

    .bss :{
        *(.bss)
    }

    PROVIDE(end = .);
}