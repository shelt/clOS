#include "isr.h"
#include "isrs.h"
#include "io.h"
#include "kernel.h" //kernel_panic
#include "vga.h" //debug

/**
 * @file isr.c
 * @brief Interrupt Service Routine handling
 * The main function isr_main() is invoked by
 * ISRs defined in isrload.s, and it itself
 * invokes C exception/request handlers.
 */

void *isr_handlers[48] = {
    // Exceptions
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    // IRQs
    0,
    isr_handle33,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0
};

const char *exception_messages[] =
{
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",

    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",

    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",

    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

void isr_main(struct isr_regs *r)
{
    void (*fn)(struct isr_regs *r) = isr_handlers[r->int_no];
    if (fn)
        fn(r);

    // If this is a hardware interrupt, EOIs are required
    if (r->int_no >= 32 && r->int_no < 48)
    {
        if (r->int_no >= 40)
            outportb(0xA0, 0x20); // send EOI to slave PIC
        outportb(0x20, 0x20); // send EOI to master PIC
    }
}
