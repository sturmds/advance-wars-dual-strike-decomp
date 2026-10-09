#include "types.h"
#include "game/event_queue.h"

int func_0207bd80(void);
void func_02066624(void);

void func_0206714c(int unused)
{
    if (func_0207bd80() == 1) {
        EventQueue_Push(27, 0);
    } else {
        func_02066624();
    }
}
