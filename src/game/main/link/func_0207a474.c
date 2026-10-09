#include "types.h"

extern u32 data_0229c88c;
extern u32 data_0229c880;

void func_0207a750(void);
void func_02079664(void);
void func_02079180(void);

/* data_0229c88c is a state of the link connection (0 to 5); its meaning is not worked out yet. */
void func_0207a474(void)
{
    func_0207a750();
    switch (data_0229c88c) {
    case 0:
        data_0229c88c = 2;
        data_0229c880 = 4;
        func_02079664();
        break;
    case 2:
    case 3:
        data_0229c88c = 2;
        data_0229c880 = 4;
        break;
    case 5:
        data_0229c880 = 4;
        func_02079180();
        break;
    }
}
