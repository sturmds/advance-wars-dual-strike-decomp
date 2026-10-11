#include "types.h"
#include "game/event_queue.h"

int Link_IsActive(void);
void func_020651ec(void);

void func_020654a0(int unused)
{
    if (Link_IsActive() == 1) {
        EventQueue_Push(33, 0);
    } else {
        func_020651ec();
    }
}
