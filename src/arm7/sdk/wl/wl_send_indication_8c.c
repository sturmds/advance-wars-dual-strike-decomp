#include "types.h"
#include "sdk/wl.h"

void func_arm7_027c80b0(u16 flags);

/* Sends host message 0x8c, which has no payload. Returns 0 when no buffer is left. */
int WifiHost_TellBeaconSent(void)
{
    WlIndication *ind = (WlIndication *)WifiBuffer_New(&WIFI_DRIVER_STATE->unk_15c, 16);

    if (ind == NULL) {
        func_arm7_027c80b0(1);
        return 0;
    }
    ind->command = 0x8c;
    ind->length = 0;
    WifiBuffer_GiveToHost(&WIFI_DRIVER_STATE->unk_15c, &ind->buffer);
    return 1;
}
