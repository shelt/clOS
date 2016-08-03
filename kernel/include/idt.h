#ifndef IDT_H
#define IDT_H
#include "common.h"

/* Defines an IDT entry */
struct idt_entry
{
    uint16_t base_lo;
    uint16_t sel;        /* Our kernel segment goes here! */
    uint8_t always0;     /* This will ALWAYS be set to 0! */
    uint8_t flags;       /* Set using the above table! */
    uint16_t base_hi;
} __attribute__((packed));

struct idt_ptr
{
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));


extern void idt_load(); // Found in descriptors.s

extern void int0();
extern void int1();
extern void int2();
extern void int3();
extern void int4();
extern void int5();
extern void int6();
extern void int7();
extern void int8();
extern void int9();
extern void int10();
extern void int11();
extern void int12();
extern void int13();
extern void int14();
extern void int15();
extern void int16();
extern void int17();
extern void int18();
extern void int19();
extern void int20();
extern void int21();
extern void int22();
extern void int23();
extern void int24();
extern void int25();
extern void int26();
extern void int27();
extern void int28();
extern void int29();
extern void int30();
extern void int31();

extern void int32();
extern void int33();
extern void int34();
extern void int35();
extern void int36();
extern void int37();
extern void int38();
extern void int39();
extern void int40();
extern void int41();
extern void int42();
extern void int43();
extern void int44();
extern void int45();
extern void int46();
extern void int47();

void idt_init();

#endif // IDT_H
