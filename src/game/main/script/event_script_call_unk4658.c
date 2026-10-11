#include "types.h"
#include "game/event_script.h"

int func_020f4658(int value);

/* Event script command: passes the command's halfword at +8 to func_020f4658
 * and moves to the next command. */
BOOL EventScript_CallUnk4658(s16 index)
{
    func_020f4658(EVENT_SCRIPT_DATA_COMMAND(index)->unk_08);
    data_022977f0[index].pc++;
    return TRUE;
}
