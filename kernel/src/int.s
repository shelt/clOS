#TODO make this file more DRY
# * combine stubs
# * why do we need 3 irp loops
#TODO improve interrupt system. Perhaps something like:
#int0:
#    cli
#    push_regdata
#    some_int-specific_fn
#    pop_regdata
# ie. it's dumb that all interrupts call the same function, when that function splits functionality again

## setup isrs that don't pop error codes themselves
.irp    param,0,1,2,3,4,5,6,7,9,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
.global int\param
int\param:
    cli
    push $0              # dummy error code to keep uniform stack frame
    push $\param
    jmp isr_common_stub
.endr

## setup isrs that DO pop error codes themselves
.irp    param,8,10,11,12,13,14
.global int\param
int\param:
    cli
    push $\param
    jmp isr_common_stub
.endr

## setup irqs
.irp    param,32,34,35,36,37,38,39,40,41,42,43,44,45,46,47 #TODO temp no 33
.global int\param
int\param:
    cli
    push $0              # dummy error code to keep uniform stack frame
    push $\param
    jmp irq_common_stub
.endr

.global int33
int33:
    cli
    push $0
    push $33
    jmp irq_common_stub


# This is our common ISR stub. It saves the processor state, sets
# up for kernel mode segments, calls the C-level fault handler,
# and finally restores the stack frame.
isr_common_stub:
    pusha
    push %ds
    push %es
    push %fs
    push %gs
    mov $0x10, %ax   # Load the Kernel Data Segment descriptor!
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %esp, %eax  # Push us the stack
    push %eax
    mov $fault_handler, %eax
    call %eax      # A special call, preserves the 'eip' register
    pop %eax
    pop %gs
    pop %fs
    pop %es
    pop %ds
    popa
    add $8, %esp # Cleans up the pushed error code and pushed ISR number
    iret           # pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP!



irq_common_stub:
    pusha
    push %ds
    push %es
    push %fs
    push %gs
    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %esp, %eax
    push %eax
    mov $irq_handler, %eax
    call %eax
    pop %eax
    pop %gs
    pop %fs
    pop %es
    pop %ds
    popa
    add $8, %esp
    iret
