#include "sdk/wl.h"

// Runs on TX done in WIFI_MODE_FRAME_TEST: sends the frame again while the
// test runs, and turns the transmitter off once WifiCmd_RunRadioTest has stopped it.
void WifiTest_ResendOrHalt(void)
{
    if (WIFI_LINK_VIEW_OF(WIFI_DRIVER_STATE)->testTxActive) {
        volatile u16 *ram = WIFI_PACKET_RAM; /* needed to match: one base register */

        ram[0] = 0;
        ram[2] = 0;
        WIFI_TX_LOC1_FRAME_ADDR |= 0x8000;
        return;
    }
    WIFI_TX_REQUEST_CLEAR = 1;
    WIFI_MAC_RUN = 0;
    WIFI_IRQ_ENABLE_MASK = 2;
    WIFI_IRQ_PENDING_FLAGS = 0xffff;
    WIFI_TX_HEADER_CONTROL = 0;
    WIFI_MAC_FORCE_POWER = 0;
}
