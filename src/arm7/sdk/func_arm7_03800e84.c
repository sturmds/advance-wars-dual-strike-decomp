#include "types.h"

void func_arm7_03800fcc(int, int, int);
int func_arm7_03800f90(int);

int func_arm7_03800e84(int a0, int a1)
{
    func_arm7_03800fcc(a0, a1, -1);
    return func_arm7_03800f90(a0);
}
