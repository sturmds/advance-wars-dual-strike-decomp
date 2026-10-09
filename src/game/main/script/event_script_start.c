#include "types.h"
#include "game/event_script.h"

void func_02014c50(void);
void func_0201ce98(void);
s16 func_0201acec(const EventScriptCommand *script);

/* Starts script in a free slot and returns the slot, or NULL when all
 * EVENT_SCRIPT_SLOTS are in use. func_02014c50 and func_0201ce98 run first
 * (they set data_022900b8 and data_0229777c; meaning unknown). */
EventScriptSlot *EventScript_Start(const EventScriptCommand *script)
{
    s16 slot;

    func_02014c50();
    func_0201ce98();
    slot = func_0201acec(NULL);
    if (slot == -1) {
        return NULL;
    }
    data_022977f0[slot].start = script;
    data_022977f0[slot].hold = 0;
    data_022977f0[slot].pc = script;
    data_022977f0[slot].wait = 0;
    return &data_022977f0[slot];
}
