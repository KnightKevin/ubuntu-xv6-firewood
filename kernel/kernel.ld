OUTPUT_ARCH( "riscv" )
ENTRY( _entry )

SECTIONS
{
    . = 0x80000000;

    .text :{
        *(.text)
    }
    PROVIDE(etxt = .);

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