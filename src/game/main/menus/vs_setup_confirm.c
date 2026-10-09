#include "types.h"
#include "game/event_queue.h"

int func_0207bd80(void);
void func_02067b0c(void);

void VsSetup_Confirm(int unused)
{
    if (func_0207bd80() == 1) {
        EventQueue_Push(17, 0);
    } else {
        func_02067b0c();
    }
}
