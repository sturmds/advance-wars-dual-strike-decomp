#include "sdk/wl.h"

int func_arm7_027cb100(int station);
void WifiStation_ReturnAssocId(u16 station);

void WifiStation_ChangeState(u16 station, u16 state)
{
    u32 irqMask = Irq_ClearEnableBits_Arm7(IRQ_MASK_WIRELESS);

    if (state < WIFI_PEER_ASSOCIATED) {
        func_arm7_027cb300(station);
        WIFI_DRIVER_STATE->stationBits.unk_06 |= 1 << station;
        if (WIFI_DRIVER_STATE->state.unk_24 == 1 && WifiStation_StateOf(station) == WIFI_PEER_ASSOCIATED) {
            WifiStation_ReturnAssocId(station);
        }
    } else {
        WIFI_DRIVER_STATE->stationBits.unk_06 &= ~(1 << station);
        if (func_arm7_027cb100(station)) {
            func_arm7_027cb324(station);
        }
    }
    WifiStation_Entry(station)->state = state;
    Irq_SetEnableBits_Arm7(irqMask);
}
