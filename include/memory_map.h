// NOTE: This file needs to be kept to strictly defines as it's shared between C and asm.

#ifndef MEMORY_MAP_H
#define MEMORY_MAP_H

// Physical addresses
#define FIQ_STACK_PA_START   0x00004000
#define IRQ_STACK_PA_START   0x00007900
#define SVC_STACK_PA_START   0x00C08000
#define SM_STACK_PA_START    0x0A827000
#define UD_STACK_PA_START    0x01008000
#define ABORT_STACK_PA_START 0x01208000

#define KERNEL_PA_START 0x00000000
#define KERNEL_PA_PT 0x000A8000 // was F
#define KERNEL_PA_TMP_TTB0 (0x4000)
#define USR_PA_START (0x00F00000)
#define PERIPHERAL_PA_START 0x20000000

//
// Virtual addresses
//

// Misc
#define KERNEL_VA_START 0x80000000
#define DYN_MEM_VA_START (KERNEL_VA_START + 0x01209000)
#define USR_VA_START 0x00000000

// Stacks
#define FIQ_STACK_VA_START   (KERNEL_VA_START + FIQ_STACK_PA_START) 
#define IRQ_STACK_VA_START   (KERNEL_VA_START + IRQ_STACK_PA_START) 
#define SVC_STACK_VA_START   (KERNEL_VA_START + SVC_STACK_PA_START) 
#define SM_STACK_VA_START    (KERNEL_VA_START + SM_STACK_PA_START)
#define UD_STACK_VA_START    (KERNEL_VA_START + UD_STACK_PA_START)  
#define ABORT_STACK_VA_START (KERNEL_VA_START + ABORT_STACK_PA_START)

// Peripherals
#define PERIPHERAL_VA_START (KERNEL_VA_START + PERIPHERAL_PA_START)
#define PERIPHERAL_VA_TIMER         (PERIPHERAL_VA_START + 0x3000)
#define PERIPHERAL_VA_IRPT          (PERIPHERAL_VA_START + 0xB200)
#define PERIPHERAL_VA_MAILBOX_BASE  (PERIPHERAL_VA_START + 0xb880)
#define PERIPHERAL_VA_RSTC          (PERIPHERAL_VA_START + 0x10001c)
#define PERIPHERAL_VA_WDOG          (PERIPHERAL_VA_START + 0x100024)
#define PERIPHERAL_VA_GPIO          (PERIPHERAL_VA_START + 0x200000)
#define PERIPHERAL_VA_UART          (PERIPHERAL_VA_START + 0x201000)
#define PERIPHERAL_VA_MINIUART      (PERIPHERAL_VA_START + 0x215000)
#define PERIPHERAL_VA_EMMC          (PERIPHERAL_VA_START + 0x300000)

//
// Linker constants
// 
#define QEMU 1

#ifdef QEMU
	#define LD_KRNL_ORIGIN 0x10000
#else
	#define LD_KRNL_ORIGIN 0x8000
#endif

#define LD_PAGE_SIZE 4096

#endif
