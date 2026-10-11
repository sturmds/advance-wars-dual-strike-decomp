#include "types.h"
#include "game/Unk_0215fe84.h"

extern u32 gWirelessState;
/* Probably an SDK call: the second argument is passed on to the callback. */
void WM_SetEntry(void (*callback)(void), int mode);
void Wireless_OnSetMode0(void);
void Wireless_OnSetMode1(void);

void func_02077f48(int mode)
{
    if (gWirelessState != 4) {
        return;
    }
    if (mode == data_0215fe84->unk_18) {
        return;
    }
    if (mode == 1) {
        WM_SetEntry(Wireless_OnSetMode1, mode);
        return;
    }
    WM_SetEntry(Wireless_OnSetMode0, mode);
}
