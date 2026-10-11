#include "types.h"
#include "sdk/mb.h"

/* Returns 1 for a child's aid (1 to 15); aid 0 is the parent. */
int MbComm_IsChildAid(u32 aid)
{
    if (aid >= 1 && aid <= DLPLAY_CHILD_SLOTS) {
        return 1;
    }
    return 0;
}
