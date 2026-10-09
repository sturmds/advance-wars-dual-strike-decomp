#include "types.h"
#include "game/Unk_02080440.h"
#include "game/script_slot.h"

extern Unk_02080440 *data_027c027c;

void func_020175e0(int slot);

/* Stops the script slot that runs script (func_020175e0 clears it and calls
 * its callback) and returns its index, or -1 when none runs it. */
s8 ScriptSlot_StopByScript(const ScriptCommand *script)
{
    u8 i;

    for (i = 0; i < 30; i++) {
        if (data_027c027c->scriptSlots[i].start == script) {
            func_020175e0(i);
            return i;
        }
    }
    return -1;
}
