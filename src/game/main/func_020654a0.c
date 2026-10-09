#include "types.h"

int func_0207bd80(void);
void EventQueue_Push(int id, int arg);
void func_020651ec(void);

void func_020654a0(int unused)
{
    if (func_0207bd80() == 1) {
        EventQueue_Push(33, 0);
    } else {
        func_020651ec();
    }
}
