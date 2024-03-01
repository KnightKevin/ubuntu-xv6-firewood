.text
_entry:
    .equ v, 1234

    li a0, v
    li a1, 0x3
    li a2, 0x4
    call start

spin:
    j spin