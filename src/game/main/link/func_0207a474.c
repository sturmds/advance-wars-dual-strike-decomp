#include "types.h"

extern u32 gWirelessState;
extern u32 data_0229c880;

void func_0207a750(void);
void func_02079664(void);
void func_02079180(void);

/* gWirelessState is a state of the link connection (0 to 5); its meaning is not worked out yet. */
void func_0207a474(void)
{
    func_0207a750();
    switch (gWirelessState) {
    case 0:
        gWirelessState = 2;
        data_0229c880 = 4;
        func_02079664();
        break;
    case 2:
    case 3:
        gWirelessState = 2;
        data_0229c880 = 4;
        break;
    case 5:
        data_0229c880 = 4;
        func_02079180();
        break;
    }
}
