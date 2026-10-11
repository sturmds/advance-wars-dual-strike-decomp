#include "types.h"
#include "game/Unk_0201d320.h"

extern Unk_0201d320 data_022977f4[];

BOOL func_0201bce0(s16 index); /* jump command */

/* Event script command: calls the test function stored in the record. When
 * it returns 0 the script goes on with the next record, otherwise it jumps
 * to the record in unk_04. */
BOOL EventCmd_JumpIfTrue(s16 index)
{
    Unk_0201d320 *script = &data_022977f4[index];

    if (((BOOL (*)(void))script->unk_00->unk_0c)() == 0) {
        script->unk_00++;
        return 1;
    }
    return func_0201bce0(index);
}
