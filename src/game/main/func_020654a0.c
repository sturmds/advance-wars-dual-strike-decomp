#include "types.h"
#include "game/event_queue.h"

int func_0207bd80(void);
void func_020651ec(void);

void func_020654a0(int unused)
{
    if (func_0207bd80() == 1) {
        EventQueue_Push(33, 0);
    } else {
        func_020651ec();
    }
}
