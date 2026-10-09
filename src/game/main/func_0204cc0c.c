#include "types.h"

int func_0204cc4c(int ch);

int func_0204cc0c(const u8 *str)
{
    int width = 0;

    while (*str != 0) {
        width += func_0204cc4c(*str);
        str++;
    }
    return width;
}
