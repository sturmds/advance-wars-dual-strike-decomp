#include "types.h"

void EventQueue_Push(int, int);

void func_0207e484(u8 *a0)
{
    EventQueue_Push(3, (*a0 - 2) & 255);
}
