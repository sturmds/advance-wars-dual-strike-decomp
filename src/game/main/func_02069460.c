#include "types.h"
#include "game/event_queue.h"

int func_0207bd80(void);
void func_02067b34(void);

void func_02069460(int unused)
{
    if (func_0207bd80() == 1) {
        EventQueue_Push(18, 0);
    } else {
        func_02067b34();
    }
}
