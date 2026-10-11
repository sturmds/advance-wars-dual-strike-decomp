#include "types.h"
#include "sdk/msl.h"

void func_02125658(Decimal *dec, int count)
{
    u8 *first = dec->digits;
    u8 *digit = first + count - 1;

    for (;;) {
        if (*digit < 9) {
            (*digit)++;
            return;
        }
        if (digit == first) {
            *digit = 1;
            dec->exp++;
            return;
        }
        *digit-- = 0;
    }
}
