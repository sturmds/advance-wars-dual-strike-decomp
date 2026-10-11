#include "sdk/pm.h"

int func_02114f8c(int kind, u16 *result);

/* Reads which backlights are on, from bits 3 (top) and 2 (bottom) of PM register 0. */
int PM_GetBackLight(int *topBackLight, int *bottomBackLight)
{
    u16 status;
    int error;

    error = func_02114f8c(0, &status);
    if (error != 0) {
        return error;
    }
    if (topBackLight != NULL) {
        *topBackLight = (status & 8) != 0;
    }
    if (bottomBackLight != NULL) {
        *bottomBackLight = (status & 4) != 0;
    }
    return error;
}
