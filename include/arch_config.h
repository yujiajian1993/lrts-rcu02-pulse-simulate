#ifndef  __ARCH_CONFIG_H
#define  __ARCH_CONFIG_H

#ifdef __cplusplus
extern "C"{
#endif

void arch_print(const char *format, ...);

#define printp  arch_print

#define EnableInterrupts CPU_IntEn()

#ifdef __cplusplus
}
#endif

#endif   /*__ARCH_CONFIG_H*/

