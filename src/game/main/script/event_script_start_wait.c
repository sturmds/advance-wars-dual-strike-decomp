#include "types.h"
#include "game/Unk_0201d320.h"

extern Unk_0201d320 data_022977f4[];

/* Wait command: loads the slot's frame counter from the command and moves
 * to the next command. func_0201d320 calls it unless data_022900b8 is 1.
 * Needed to match: the slot fields are reached through data_022977f4 and
 * data_022977fc (EventScriptSlot.pc and .wait), since the original loads
 * each from its own symbol. */
BOOL EventScript_StartWait(s16 index)
{
    data_022977f4[index].unk_08 = data_022977f4[index].unk_00->unk_0c;
    data_022977f4[index].unk_00++;
    return FALSE;
}
