
#define __inline
#define __inline__
#define __attribute__(x)
#define __extension__
#define __restrict
#define __except
#define __volatile
#define __volatile__
#define typeof
#define __typeof__
#define __extension__
#define __external__
#define __complex__
#define __real__
#define __imag__

/* CCI */
#define __at(x)
#define __section(x)
#define __bank(x)
#define __align(x)
#define __interrupt(x)
#define __far
#define __near
#define __persistent
#define __xdata
#define __ydata
#define __eeprom
#define __pack
#define __deprecate


/* built-in Function List */

unsigned long __builtin_section_begin(const char *);
unsigned long __builtin_section_end(const char *);
unsigned long __builtin_section_size(const char *);
unsigned int __builtin_get_isr_state(void);
void __builtin_set_isr_state(unsigned int);
void __builtin_software_breakpoint(void);
void __builtin_enable_interrupts(void);
unsigned int __builtin_bcc0(unsigned int, unsigned int, unsigned int);
unsigned int __builtin_mfc0(unsigned int, unsigned int, unsigned int);
void __builtin_mtc0(unsigned int, unsigned int, unsigned int);
unsigned int __builtin_mxc0(unsigned int, unsigned int, unsigned int);
unsigned int __builtin_bsc0(unsigned int, unsigned int, unsigned int);
unsigned int __builtin_bcsc0(unsigned int, unsigned int, unsigned int);

typedef unsigned short char16_t;
typedef unsigned int char32_t;

#define _HAS_CHAR16_T_LANGUAGE_SUPPORT 0







				   









