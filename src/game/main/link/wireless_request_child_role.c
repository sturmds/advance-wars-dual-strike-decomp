#include "types.h"

extern u32 data_0229c880;
extern u32 data_0229c88c;

void func_02079180(void);
void func_02079664(void);

/* Asks for the child role (data_0229c880 = 3). From the idle state it starts
 * the step chain; while a step runs it only records the role; after an
 * error (states 4 and 5) it restarts through func_02079180. Link_Start calls it. */
void Wireless_RequestChildRole(void)
{
    switch (data_0229c88c) {
    case 0:
        data_0229c88c = 2;
        data_0229c880 = 3;
        func_02079664();
        break;
    case 2:
    case 3:
        data_0229c880 = 3;
        break;
    case 4:
        data_0229c880 = 3;
        func_02079180();
        break;
    case 5:
        data_0229c880 = 3;
        func_02079180();
        break;
    }
}
