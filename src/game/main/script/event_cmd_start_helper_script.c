#include "types.h"
#include "game/Unk_0201d320.h"
#include "game/event_script.h"

extern Unk_0201d320 data_022977f4[];
extern const EventScriptCommand data_021584d8[];
extern u8 data_022900b8;

/* Event script command: starts the script at data_021584d8 in a free slot and
 * stores the current record in the new slot's unk_14, unless the skip flag
 * data_022900b8 is 1. Then goes on. The script's purpose is unknown. */
BOOL EventCmd_StartHelperScript(s16 index)
{
    EventScriptSlot *started;

    if (data_022900b8 != 1) {
        started = EventScript_Start(data_021584d8);
        started->unk_14 = (const EventScriptCommand *)data_022977f4[index].unk_00;
    }
    data_022977f4[index].unk_00++;
    return TRUE;
}
