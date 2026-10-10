#include "types.h"
#include "game/Unk_ov005_0235abfc.h"

/* 91.67%: one trailing "bx lr" too many. The original returns from inside
 * the loop through predicated returns. */
int func_ov005_0235ad60(int work, Unk_ov005_0235abfc *entry)
{
    int found = 0;

    for (;;) {
        if (entry->id == 0) {
            break;
        }
        if (entry->task->unk_30 == 1) {
            found = 1;
            break;
        }
        entry++;
    }
    return found;
}
