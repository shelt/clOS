# This file is the assembly counterpart
# of gdt.c.

# This will set up our new segment registers. We need to do
# something special in order to set CS. We do what is called a
# far jump. A jump that includes a segment as well as an offset.
.global gdt_load
gdt_load:
    lgdt (gdtp)        # found in gdt.c
    # flush registers (todo maybe do this in the function we jump to for cs register)
    mov $0x10, %ax       # 0x10 is the offset to data segment
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %ax, %ss
    ljmp $0x08, $update_cs  # 0x08 is the offset to code segment (far jump, done to update %cs)
update_cs:
    ret
