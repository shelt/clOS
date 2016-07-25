#include "isr.h"
#include "io.h"
#include "kernel.h" //kernel_panic

void *isr_irq_table[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

void isr_irq_set(int irq, void (*fn)(struct isr_regs *r))
{
    isr_irq_table[irq] = fn;
}

void isr_irq_unset(int irq)
{
    isr_irq_table[irq] = 0;
}

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
    if (r->int_no < 32) // This is a processor exception
    {
        kernel_panic(exception_messages[r->int_no]);
        for (;;); //TODO don't just die
    }
    else
    {
        // This is an IRQ. Check for a custom ISR...
        void (*fn)(struct isr_regs *r) = isr_irq_table[r->int_no - 32];
        if (fn)
            fn(r);

        // If this is a hardware interrupt, EOIs are required
        if (r->int_no < 48)
        {
            if (r->int_no >= 40)
                outportb(0xA0, 0x20); // send EOI to slave PIC
            outportb(0x20, 0x20); // send EOI to master PIC
        }
    }
}
