#include "types.h"

void Wireless_OnScan(void *event);
int func_021178d0(void (*callback)(void *event), int arg);

extern int data_0229c8b4;

/* Starts a request that reports to Wireless_OnScan; true when func_021178d0
 * returns 2. The callers fall through to the failure path on false. */
BOOL Wireless_StartRequest(void)
{
    return func_021178d0(Wireless_OnScan, data_0229c8b4) == 2;
}
