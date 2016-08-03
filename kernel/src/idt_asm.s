# This file is the assembly counterpart
# of idt.c.

.global idt_load
idt_load:
    lidt (idtp)        # found in idt.c
    ret

