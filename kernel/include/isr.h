#ifndef ISR_H
#define ISR_H

/* what the stack looks like in the isr */
struct isr_regs
{
    unsigned int gs, fs, es, ds;      /* pushed the segs last */
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;  /* pushed by 'pusha' */
    unsigned int int_no, err_code;    /* our 'push byte #' and ecodes do this */
    unsigned int eip, cs, eflags, useresp, ss;   /* pushed by the processor automatically */ 
};

void isr_irq_set(int irq, void (*fn)(struct isr_regs *r));
void isr_irq_unset(int irq);


#endif // ISR_H
