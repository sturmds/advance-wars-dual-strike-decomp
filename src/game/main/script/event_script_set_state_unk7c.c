#include "types.h"
#include "game/event_script.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

/* Event script command: sets the map state field unk_7c (purpose unknown) to the
 * command's value and moves to the next command. */
BOOL EventScript_SetStateUnk7c(s16 index)
{
    data_027c027c->unk_48[data_027c0284]->unk_7c = EVENT_SCRIPT_DATA_COMMAND(index)->value;
    data_022977f0[index].pc++;
    return TRUE;
}
