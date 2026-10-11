#include "types.h"
#include "game/Unk_02080440.h"
#include "game/script_slot.h"

extern Unk_02080440 *data_027c027c;

BOOL ScriptSlot_AnyPending(void);
void func_020171cc(void);

/* Flags every used slot, then calls func_020171cc until no flagged slot is left.
 * func_020171cc runs the flagged slots through ScriptSlot_RunPending. */
void ScriptSlot_RunAll(void)
{
    u8 i;

    for (i = 0; i < 30; i++) {
        if (data_027c027c->scriptSlots[i].start != NULL) {
            data_027c027c->scriptSlots[i].unk_12 |= 1;
        }
    }
    do {
        func_020171cc();
    } while (ScriptSlot_AnyPending());
}
