#include "types.h"
#include "sdk/msl_string.h"

char *strcpy(char *dest, const char *src)
{
    char *out = dest;
    u32 misalign = (u32)src & 3;
    u32 word;

    if (((u32)dest & 3) == misalign) {
        if (misalign != 0) {
            if ((*out = *src) == 0) {
                return dest;
            }
            for (misalign = 3 - misalign; misalign != 0; misalign--) {
                if ((*++out = *++src) == 0) {
                    return dest;
                }
            }
            out++;
            src++;
        }

        word = *(u32 *)src;
        if (((word + 0xFEFEFEFF) & 0x80808080) == 0) {
            out -= 4;
            do {
                *(u32 *)(out += 4) = word;
                word = *(u32 *)(src += 4);
            } while (((word + 0xFEFEFEFF) & 0x80808080) == 0);
            out += 4;
        }
    }

    if ((*out = *src) == 0) {
        return dest;
    }
    while ((*++out = *++src) != 0) {
    }
    return dest;
}
