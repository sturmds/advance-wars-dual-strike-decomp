#include "types.h"

void func_02086b1c(int a0);
void func_02086b58(void);

void func_02087714(u8 *a0)
{
    func_02086b1c((*a0 - 8) & 255);
    func_02086b58();
}
