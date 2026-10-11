#ifndef SDK_MSL_WCHAR_H
#define SDK_MSL_WCHAR_H

#include "types.h"

typedef u16 wchar_t;

/* Character conversion functions of the locale's ctype part. */
typedef struct MslCtype {
    void *mbtowc;
    int (*wctomb)(char *dest, wchar_t wc);
} MslCtype;

typedef struct MslLocale {
    void *unk_00;
    void *unk_04;
    MslCtype *ctype;
} MslLocale;

extern MslLocale gMslLocale;

/* Returns the number of bytes written to dest, or -1 if wc has no multibyte form. */
int wctomb(char *dest, wchar_t wc);
/* Returns the number of bytes written to dest, not counting the final null. */
u32 wcstombs(char *dest, const wchar_t *src, u32 count);
/* Locale hook for the "C" locale: copies one byte; returns 0 for a null byte, 1 otherwise, -1 if count is 0. */
int __mbtowc_noconv(wchar_t *dest, const char *src, u32 count);
char *strncpy(char *dest, const char *src, u32 count);

#endif
