#include "types.h"
#include "game/event_queue.h"

int func_0207bd80(void);
void func_02065214(void);

void func_02065464(int unused)
{
    if (func_0207bd80() == 1) {
        EventQueue_Push(32, 0);
    } else {
        func_02065214();
    }
}
