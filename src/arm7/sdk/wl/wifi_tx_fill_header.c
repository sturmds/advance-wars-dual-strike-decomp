#include "sdk/mi.h"
#include "sdk/wl.h"

u16 func_arm7_027c8c00(void);

// Clears the header of a frame to send and fills in the rate and the
// addresses: dest, then our MAC address, then the BSSID.
void WifiTx_FillHeader(WlTxFrame* frame, const u16* dest) {
  {
    volatile u16 zero =
        0; /* needed to match: the zero goes through the stack */
    Mem_FillHalfwords_Arm7(zero, frame, 0x2c); /* everything before body */
  }
  frame->rate = func_arm7_027c8c00();
  WifiMac_CopyAddressTriple(frame->addr1, dest, WIFI_DRIVER_STATE->macAddress,
                            WIFI_DRIVER_STATE->state.unk_7c);
}
