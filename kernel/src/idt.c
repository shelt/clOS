#include "idt.h"
#include "isr.h"
#include "isrs.h"
#include "str.h"
#include "io.h"

// Actual IDT in memory
struct idt_entry idt[256];
struct idt_ptr idtp;

// IDT descriptor creation
void idt_set_gate(unsigned char i, unsigned long base, unsigned short sel, unsigned char flags)
{
    /* The interrupt routine's base address */
    idt[i].base_lo = (base & 0xFFFF);
    idt[i].base_hi = (base >> 16) & 0xFFFF;

    /* The segment or 'selector' that this IDT entry will use
    *  is set here, along with any access flags */
    idt[i].sel = sel;
    idt[i].always0 = 0;
    idt[i].flags = flags;
}

void idt_init()
{
    /* Sets the special IDT pointer up, just like in 'gdt.c' */
    idtp.limit = (sizeof (struct idt_entry) * 256) - 1;
    idtp.base = (unsigned int)&idt;

    /* Clear out the entire IDT, initializing it to zeros */
    memset(&idt, 0, sizeof(struct idt_entry) * 256);

    /* Normally, IRQs 0 to 7 are mapped to entries 8 to 15. This
    *  is a problem in protected mode, because IDT entry 8 is a
    *  Double Fault! Without remapping, every time IRQ0 fires,
    *  you get a Double Fault Exception, which is NOT actually
    *  what's happening. We send commands to the Programmable
    *  Interrupt Controller (PICs - also called the 8259's) in
    *  order to make IRQ0 to 15 be remapped to IDT entries 32 to
    *  47 */
    outportb(0x20, 0x11);
    outportb(0xA0, 0x11);
    outportb(0x21, 0x20);
    outportb(0xA1, 0x28);
    outportb(0x21, 0x04);
    outportb(0xA1, 0x02);
    outportb(0x21, 0x01);
    outportb(0xA1, 0x01);
    outportb(0x21, 0x0);
    outportb(0xA1, 0x0);

    // Faults
    idt_set_gate(0, (unsigned)int0, 0x08, 0x8E);
    idt_set_gate(1, (unsigned)int1, 0x08, 0x8E);
    idt_set_gate(2, (unsigned)int2, 0x08, 0x8E);
    idt_set_gate(3, (unsigned)int3, 0x08, 0x8E);
    idt_set_gate(4, (unsigned)int4, 0x08, 0x8E);
    idt_set_gate(5, (unsigned)int5, 0x08, 0x8E);
    idt_set_gate(6, (unsigned)int6, 0x08, 0x8E);
    idt_set_gate(7, (unsigned)int7, 0x08, 0x8E);
    idt_set_gate(8, (unsigned)int8, 0x08, 0x8E);
    idt_set_gate(9, (unsigned)int9, 0x08, 0x8E);
    idt_set_gate(10, (unsigned)int10, 0x08, 0x8E);
    idt_set_gate(11, (unsigned)int11, 0x08, 0x8E);
    idt_set_gate(12, (unsigned)int12, 0x08, 0x8E);
    idt_set_gate(13, (unsigned)int13, 0x08, 0x8E);
    idt_set_gate(14, (unsigned)int14, 0x08, 0x8E);
    idt_set_gate(15, (unsigned)int15, 0x08, 0x8E);
    idt_set_gate(16, (unsigned)int16, 0x08, 0x8E);
    idt_set_gate(17, (unsigned)int17, 0x08, 0x8E);
    idt_set_gate(18, (unsigned)int18, 0x08, 0x8E);
    idt_set_gate(19, (unsigned)int19, 0x08, 0x8E);
    idt_set_gate(20, (unsigned)int20, 0x08, 0x8E);
    idt_set_gate(21, (unsigned)int21, 0x08, 0x8E);
    idt_set_gate(22, (unsigned)int22, 0x08, 0x8E);
    idt_set_gate(23, (unsigned)int23, 0x08, 0x8E);
    idt_set_gate(24, (unsigned)int24, 0x08, 0x8E);
    idt_set_gate(25, (unsigned)int25, 0x08, 0x8E);
    idt_set_gate(26, (unsigned)int26, 0x08, 0x8E);
    idt_set_gate(27, (unsigned)int27, 0x08, 0x8E);
    idt_set_gate(28, (unsigned)int28, 0x08, 0x8E);
    idt_set_gate(29, (unsigned)int29, 0x08, 0x8E);
    idt_set_gate(30, (unsigned)int30, 0x08, 0x8E);
    idt_set_gate(31, (unsigned)int30, 0x08, 0x8E);

    // Requests
    idt_set_gate(32, (unsigned)int32, 0x08, 0x8E);
    idt_set_gate(33, (unsigned)int33, 0x08, 0x8E);
    idt_set_gate(34, (unsigned)int34, 0x08, 0x8E);
    idt_set_gate(35, (unsigned)int35, 0x08, 0x8E);
    idt_set_gate(36, (unsigned)int36, 0x08, 0x8E);
    idt_set_gate(37, (unsigned)int37, 0x08, 0x8E);
    idt_set_gate(38, (unsigned)int38, 0x08, 0x8E);
    idt_set_gate(39, (unsigned)int39, 0x08, 0x8E);
    idt_set_gate(40, (unsigned)int40, 0x08, 0x8E);
    idt_set_gate(41, (unsigned)int41, 0x08, 0x8E);
    idt_set_gate(42, (unsigned)int42, 0x08, 0x8E);
    idt_set_gate(43, (unsigned)int43, 0x08, 0x8E);
    idt_set_gate(44, (unsigned)int44, 0x08, 0x8E);
    idt_set_gate(45, (unsigned)int45, 0x08, 0x8E);
    idt_set_gate(46, (unsigned)int46, 0x08, 0x8E);
    idt_set_gate(47, (unsigned)int47, 0x08, 0x8E);

    // Request ISRs
    isr_irq_set(1, isr_irq1);
    
    /* Points the processor's internal register to the new IDT */
    idt_load();
}

