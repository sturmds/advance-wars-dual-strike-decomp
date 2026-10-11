#include "types.h"

/* Sets two values for kind 3 (24 and 48) or kind 4 (8 and 16); other kinds
 * leave them alone. Name kept: the caller (func_020d9494) is not decompiled
 * yet. */
void func_020d9294(int *a, int *b, int kind)
{
    if (kind == 3) {
        *a = 24;
        *b = 48;
    }
    if (kind == 4) {
        *a = 8;
        *b = 16;
    }
}
