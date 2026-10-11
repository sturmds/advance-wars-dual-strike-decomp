#include "types.h"

extern u32 data_0229c880;
extern u32 gWirelessState;

void func_0207a750(void);
void func_02079180(void);
void func_02079664(void);

/* Asks for the parent role (data_0229c880 = 4): fills in the parent's own
 * data first, then starts the step chain or records the role, like
 * Wireless_RequestChildRole. */
void Wireless_RequestParentRole(void)
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
        data_0229c880 = 4;
        break;
    case 5:
        data_0229c880 = 4;
        func_02079180();
        break;
    }
}
