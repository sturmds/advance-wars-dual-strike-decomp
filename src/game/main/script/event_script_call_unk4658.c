#include "types.h"
#include "game/event_script.h"

int SceneMode_Request(int value);

/* Event script command: passes the command's halfword at +8 to SceneMode_Request
 * and moves to the next command. */
BOOL EventScript_CallUnk4658(s16 index)
{
    SceneMode_Request(EVENT_SCRIPT_DATA_COMMAND(index)->unk_08);
    data_022977f0[index].pc++;
    return TRUE;
}
