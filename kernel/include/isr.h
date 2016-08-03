#ifndef ISR_H
#define ISR_H
#include "common.h"

/* what the stack looks like in the isr */
struct isr_regs
{
    uint32_t gs, fs, es, ds;      /* pushed the segs last */
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;  /* pushed by 'pusha' */
    uint32_t int_no, err_code;    /* our 'push byte #' and ecodes do this */
    uint32_t eip, cs, eflags, useresp, ss;   /* pushed by the processor automatically */ 
};

void isr_irq_set(int irq, void (*fn)(struct isr_regs *r));
void isr_irq_unset(int irq);


#endif // ISR_H
