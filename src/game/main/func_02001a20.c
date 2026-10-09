#include "types.h"

void func_02001a30(int, int, int);

void func_02001a20(int a0, int a1, int a2)
{
    func_02001a30(a0, a1, a2 & 127);
}
