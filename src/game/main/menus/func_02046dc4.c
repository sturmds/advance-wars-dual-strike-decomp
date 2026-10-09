#include "types.h"

void MenuButton_Add(int, int, void *, int);
int func_02047960(void);
extern u8 data_0215ac9c[];

void func_02046dc4(void)
{
    MenuButton_Add(0, 32, data_0215ac9c, 0);
    if (func_02047960() == 1) {
        MenuButton_Add(0, 96, data_0215ac9c, 1);
    }
}
