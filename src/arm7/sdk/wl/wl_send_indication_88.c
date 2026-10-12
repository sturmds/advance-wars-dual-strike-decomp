#include "types.h"
#include "sdk/wl.h"

void func_arm7_027c80b0(u16 flags);
void func_arm7_027c90ec(void *dest, const void *src); /* copies a 6-byte MAC address */

/* Sends host message 0x88 with a MAC address and a value. Returns 0 when no buffer is left. */
int WifiHost_TellDisassociated(const u8 *mac, u16 value)
{
    WlIndication *ind = (WlIndication *)WifiBuffer_New(&WIFI_DRIVER_STATE->unk_15c, 24);

    if (ind == NULL) {
        func_arm7_027c80b0(1);
        return 0;
    }
    ind->command = 0x88;
    ind->length = 4;
    func_arm7_027c90ec(ind->peer.mac, mac);
    ind->peer.value = value;
    WifiBuffer_GiveToHost(&WIFI_DRIVER_STATE->unk_15c, &ind->buffer);
    return 1;
}
