#include "types.h"
#include "game/Unk_02080440.h"
#include "game/script_slot.h"

extern Unk_02080440 *data_027c027c;

s8 func_02017130(const ScriptCommand *script);
void func_02017734(const ScriptCommand *script, u8 slot, int arg);

/* Starts script in a free script slot (func_02017130 with NULL finds one) and
 * returns the slot, or NULL when all 30 are in use. arg goes to func_02017734. */
ScriptSlot *ScriptSlot_Start(const ScriptCommand *script, int arg)
{
    s8 slot = func_02017130(NULL);

    if (slot == -1) {
        return NULL;
    }
    func_02017734(script, slot, arg);
    return &data_027c027c->scriptSlots[slot];
}
