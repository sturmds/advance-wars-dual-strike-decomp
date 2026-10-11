#include "types.h"
#include "game/Unk_0215fe84.h"

extern u32 data_0229c880;
extern u32 gWirelessState;
extern void *data_0229c8ac;

void func_02118bb0(void *sharing);

/* Stops data sharing when it runs and the link state is 4 or 5. */
void Wireless_EndDataSharing(void)
{
    if (data_0215fe84->dataSharingActive == 0) {
        return;
    }
    if (gWirelessState != data_0229c880) {
        return;
    }
    switch (data_0229c880) {
    case 4:
    case 5:
        func_02118bb0(data_0229c8ac);
        data_0215fe84->dataSharingActive = 0;
        break;
    }
}
