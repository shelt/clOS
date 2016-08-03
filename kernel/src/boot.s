# Generic kernel init sections.
# See linker.ld to see how things are laid out.

# Constants
.set ALIGN,      1<<0
.set MEMINFO,    1<<1
.set FLAGS,      ALIGN | MEMINFO
.set MAGIC,      0x1BADB002
.set CHECKSUM,   -(MAGIC + FLAGS)
.set STACK_SIZE, 16384 # 16 KiB

### MULTIBOOT HEADER ###
.section .mboot_header
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

### STACK ###
.section .bss, "aw", @nobits
stack_bottom:
.skip STACK_SIZE
stack_top:

### CODE ###
.section .text
.global kernel_mode
.type kernel_mode, @function
kernel_mode:
    # setup stack
    movl $stack_top, %esp

    # invoke kernel
    push %ebx         # mboot struct
    push %eax         # mboot magic
    call kernel_main  # found in kernel.c

    # halt forever
    cli
    hlt
.hang:
    jmp .hang

# Set the size of the kernel_mode symbol to the current location '.' minus its start.
# This is useful for debugging or call tracing.
.size kernel_mode, . - kernel_mode
