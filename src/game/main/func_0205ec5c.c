#include "types.h"
#include "game/event_queue.h"

int func_0207bd80(void);
void MapSelect_PrevList(void);

void func_0205ec5c(int unused)
{
    if (func_0207bd80() == 1) {
        EventQueue_Push(1, 0);
    } else {
        MapSelect_PrevList();
    }
}
