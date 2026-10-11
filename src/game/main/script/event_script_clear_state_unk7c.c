#include "types.h"
#include "game/event_script.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

/* Event script command: clears the map state field unk_7c (purpose unknown) and
 * moves to the next command. */
BOOL EventScript_ClearStateUnk7c(s16 index)
{
    data_027c027c->unk_48[data_027c0284]->unk_7c = 0;
    data_022977f0[index].pc++;
    return TRUE;
}
