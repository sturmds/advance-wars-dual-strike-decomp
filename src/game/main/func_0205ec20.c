#include "types.h"
#include "game/event_queue.h"

int Link_IsActive(void);
void MapSelect_NextList(void);

void func_0205ec20(int unused)
{
    if (Link_IsActive() == 1) {
        EventQueue_Push(2, 0);
    } else {
        MapSelect_NextList();
    }
}
