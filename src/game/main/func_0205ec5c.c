#include "types.h"
#include "game/event_queue.h"

int Link_IsActive(void);
void MapSelect_PrevList(void);

void func_0205ec5c(int unused)
{
    if (Link_IsActive() == 1) {
        EventQueue_Push(1, 0);
    } else {
        MapSelect_PrevList();
    }
}
