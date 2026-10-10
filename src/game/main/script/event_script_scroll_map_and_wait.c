#include "types.h"
#include "game/event_script.h"

void func_020f5878(s16 x, s16 y);
void func_0201d480(void);

/* Starts a map scroll (guessed from func_020f54a0's callees) with the command's
 * two halfwords, then holds the script until the scroll script ends:
 * func_0201d480 clears the hold. */
BOOL EventScript_ScrollMapAndWait(s16 index)
{
    func_020f5878(data_022977f0[index].pc->unk_08, data_022977f0[index].pc->unk_0a);
    data_022977f0[index].hold = (u32)func_0201d480;
    data_022977f0[index].pc++;
    return FALSE;
}
