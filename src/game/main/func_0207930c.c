#include "types.h"

extern u32 data_0229c88c;

extern int func_02117664(void (*callback)(void));
extern void Wireless_OnIndication(void);

/* Returns TRUE when the call returns 0; otherwise sets data_0229c88c to 1 and returns FALSE. */
BOOL func_0207930c(void)
{
    if (func_02117664(Wireless_OnIndication) != 0) {
        data_0229c88c = 1;
        return FALSE;
    }
    return TRUE;
}
