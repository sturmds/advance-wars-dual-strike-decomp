#include "sdk/wl.h"

void func_arm7_037f9380(int mode); /* switches WlLink.mode */
u32 WifiFlash_ValidateRadioBlock(u32 *crcs);

int WifiCmd_GoIdleMode(WlRequest *request, WlReply *reply)
{
    int result = 1;

    reply->status = result; /* needed to match: r0 keeps the 1 for the return */
    if (WIFI_DRIVER_STATE->state.mode <= 32 && WIFI_DRIVER_STATE->state.unk_2a == 0) {
        if (WifiFlash_ValidateRadioBlock(NULL) != WIFI_RADIO_BLOCK_VALID) {
            return 14;
        }
        func_arm7_037f9380(WIFI_MODE_RADIO_ON);
        result = 0;
    }
    return result;
}
