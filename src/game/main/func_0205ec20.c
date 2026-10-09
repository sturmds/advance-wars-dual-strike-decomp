#include "types.h"
#include "game/event_queue.h"

int func_0207bd80(void);
void MapSelect_NextList(void);

void func_0205ec20(int unused)
{
    if (func_0207bd80() == 1) {
        EventQueue_Push(2, 0);
    } else {
        MapSelect_NextList();
    }
}
