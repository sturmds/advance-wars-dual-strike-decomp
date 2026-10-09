#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"

BOOL func_020e8ae4(void);
Unk_02023724 *func_02023724(const void *script);
void *Task_Create(const void *script, void *parentOrList);

extern Unk_02080440 *data_027c027c;
extern Unk_027c0288 data_027c0288;
extern const u32 data_02168b94[];     /* task script of (opcode, argument) pairs */

/* On the first of the two game states (data_027c0288), while visionRanksOff is 1,
 * starts the data_02168b94 task on top-level list 3 unless it is already
 * running. */
void func_020e88f8(void)
{
    if (data_027c027c != data_027c0288.unk_00) {
        return;
    }
    if (!func_020e8ae4()) {
        return;
    }
    if (func_02023724(data_02168b94) != NULL) {
        return;
    }
    Task_Create(data_02168b94, (void *)3);
}
