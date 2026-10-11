#include "types.h"
#include "sdk/msl.h"

int __must_round(Decimal *dec, int count)
{
    u8 *rest = dec->digits + count;
    u8 *end;

    if (dec->digits[count] > 5) {
        return 1;
    }
    if (dec->digits[count] < 5) {
        return -1;
    }
    end = dec->digits + dec->length;
    for (rest++; rest < end; rest++) {
        if (*rest != 0) {
            return 1;
        }
    }
    /* Exactly half: round to an even last digit. */
    return (dec->digits[count - 1] & 1) ? 1 : -1;
}
