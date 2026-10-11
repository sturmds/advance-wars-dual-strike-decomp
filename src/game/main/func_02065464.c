#include "types.h"
#include "game/event_queue.h"

int Link_IsActive(void);
void func_02065214(void);

void func_02065464(int unused)
{
    if (Link_IsActive() == 1) {
        EventQueue_Push(32, 0);
    } else {
        func_02065214();
    }
}
