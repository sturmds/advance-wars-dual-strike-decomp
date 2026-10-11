#include "types.h"
#include "sdk/msl_wchar.h"

int __mbtowc_noconv(wchar_t *dest, const char *src, u32 count)
{
    if (src == NULL) {
        return 0;
    }
    if (count == 0) {
        return -1;
    }
    if (dest != NULL) {
        *dest = (u8)*src;
    }
    if (*src == 0) {
        return 0;
    }
    return 1;
}
