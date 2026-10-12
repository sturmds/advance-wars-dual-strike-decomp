#ifndef STDARG_H
#define STDARG_H

/* Variable arguments for mwccarm. The build uses -nostdinc, so this replaces
 * the compiler's own header. The arguments after the last named one are on
 * the stack in 4-byte slots, so va_start takes the address of the last named
 * argument, rounded down to 4 bytes, and steps past it. */
typedef char* va_list;

#define VA_SLOT_SIZE(n) ((((unsigned long)(n)) + 3U) & ~3U)
#define va_start(ap, last)                                  \
  ((ap) = (va_list)((char*)((unsigned long)&(last) & ~3U) + \
                    VA_SLOT_SIZE(sizeof(last))))
#define va_arg(ap, type) \
  (*(type*)(((ap) += VA_SLOT_SIZE(sizeof(type))) - VA_SLOT_SIZE(sizeof(type))))
#define va_end(ap) ((void)0)

#endif
