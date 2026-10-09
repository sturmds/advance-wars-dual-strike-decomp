#include "types.h"
#include "game/event_script.h"

void func_0201ac94(EventScriptSlot *slot);

/* Frees script slot number slot. */
void EventScript_Stop(s16 slot)
{
    func_0201ac94(&data_022977f0[slot]);
}
