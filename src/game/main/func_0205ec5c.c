#include "types.h"

int func_0207bd80(void);
void EventQueue_Push(int id, int arg);
void func_0205d2c0(void);

void func_0205ec5c(int unused)
{
    if (func_0207bd80() == 1) {
        EventQueue_Push(1, 0);
    } else {
        func_0205d2c0();
    }
}
