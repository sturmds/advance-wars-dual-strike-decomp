#include "types.h"

void EventQueue_Push(int kind, int arg);

void func_020877e0(u8 *value)
{
    EventQueue_Push(3, *value);
}
