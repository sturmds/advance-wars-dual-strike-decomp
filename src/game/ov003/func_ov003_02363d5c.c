#include "types.h"

int func_ov003_02363d9c(int ch);

int func_ov003_02363d5c(const u8 *str)
{
    int width = 0;

    while (*str != 0) {
        width += func_ov003_02363d9c(*str);
        str++;
    }
    return width;
}
