#include "types.h"

void func_020789a8(void *event);
int func_021178d0(void (*callback)(void *event), int arg);

extern int data_0229c8b4;

/* Starts a request that reports to func_020789a8; true when func_021178d0
 * returns 2. The callers fall through to the failure path on false. */
BOOL Wireless_StartRequest(void)
{
    return func_021178d0(func_020789a8, data_0229c8b4) == 2;
}
