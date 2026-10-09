#include "types.h"
#include "game/event_queue.h"

int func_0207bd80(void);
void func_0205d110(void);

void func_0205ec20(int unused)
{
    if (func_0207bd80() == 1) {
        EventQueue_Push(2, 0);
    } else {
        func_0205d110();
    }
}
