#include "types.h"
#include "game/Unk_0201d320.h"
#include "game/keypad.h"

extern Unk_0201d320 data_022977f4[];

void func_0201ce98(void);

/* Event script command: waits until one of the keys in the record's mask is
 * down (gKeypad->unk_0c). Then it calls the record's function, moves to the
 * next record and ends the slot's run for this frame. */
BOOL EventCmd_WaitForKeys(s16 index)
{
    Unk_0201d320 *script = &data_022977f4[index];
    Unk_0201d320_Entry *entry = script->unk_00;
    void (*run)() = entry->unk_04.run;

    if (gKeypad->unk_0c & entry->unk_0c) {
        run();
    } else {
        return 0;
    }
    func_0201ce98();
    script->unk_00++;
    return 0;
}
