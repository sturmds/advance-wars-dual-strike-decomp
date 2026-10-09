#include "types.h"

int func_0207bd80(void);
void EventQueue_Push(int id, int arg);
void func_02067b0c(void);

void VsSetup_Confirm(int unused)
{
    if (func_0207bd80() == 1) {
        EventQueue_Push(17, 0);
    } else {
        func_02067b0c();
    }
}
