#include "types.h"
#include "game/Unk_0215fe84.h"

extern u32 data_0229c880;
extern u32 gWirelessState;

void func_02079180(void);

/* Called when link play stops. While scanning (gWirelessState 2 or 3) it only
 * stops the scan from restarting; in states 4 and 5 it also runs
 * func_02079180 and forgets the children. */
void Wireless_Stop(void)
{
    switch (gWirelessState) {
    case 2:
    case 3:
        data_0229c880 = 0;
        data_0215fe84->scanning = 0;
        break;
    case 4:
    case 5:
        data_0229c880 = 0;
        func_02079180();
        data_0215fe84->childAids = 0;
        data_0215fe84->childCount = 0;
        break;
    }
}
