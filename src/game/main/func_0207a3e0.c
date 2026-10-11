#include "types.h"

extern u32 gWirelessState;
extern u32 data_0229c880;

void func_02079664(void);
void Wireless_StartRescan(void);

/* gWirelessState is a state of the link connection (0 to 5); its meaning is not worked out yet. */
void func_0207a3e0(void)
{
    switch (gWirelessState) {
    case 0:
        gWirelessState = 2;
        data_0229c880 = 3;
        func_02079664();
        break;
    case 2:
    case 3:
        data_0229c880 = 3;
        break;
    case 4:
    case 5:
        data_0229c880 = 3;
        Wireless_StartRescan();
        break;
    }
}
