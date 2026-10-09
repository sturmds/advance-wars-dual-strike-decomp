#include "types.h"

int func_0207bd80(void);
void EventQueue_Push(int id, int arg);
void func_02067b34(void);

void func_02069460(int unused)
{
    if (func_0207bd80() == 1) {
        EventQueue_Push(18, 0);
    } else {
        func_02067b34();
    }
}
