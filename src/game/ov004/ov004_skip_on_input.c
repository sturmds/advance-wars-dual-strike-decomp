#include "types.h"
#include "game/Unk_ov004_022bf158.h"

BOOL func_020fd7b0(void);
BOOL func_020fd790(void);
void Sound_PlaySe(u32 soundId);
void Task_Destroy(void *task);

/* On a touch or a press of A or Start: stops the two tasks and jumps the
 * script to step 1000. Returns TRUE when it skipped. */
BOOL Ov004_SkipOnInput(Unk_ov004_022bf158 *state, Ov004Task *task) {
    if (func_020fd7b0() || func_020fd790()) {
        Sound_PlaySe(0x10006);
        state->unk_004 = 0;
        state->unk_008 = 1;
        if (state->unk_810 != 0) {
            Task_Destroy(state->unk_810);
        }
        if (state->unk_814 != 0) {
            Task_Destroy(state->unk_814);
        }
        task->step = 1000;
        return TRUE;
    }
    return FALSE;
}
