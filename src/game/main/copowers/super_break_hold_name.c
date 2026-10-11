#include "types.h"
#include "game/task.h"

void func_02002620(int x, int y, int arg2, int arg3, int list, BOOL flip);
s16 func_0202a39c(u8 slot);
void func_0202a2c4(u8 slot, u16 x, int y, int attr, int arg);
void Task_ResumeScript(Task *task);

/* The frame counter shares its slot with Task.unk_4c. */
#define FRAME(task) (*(s16 *)&(task)->unk_4c)

#define HOLD_FRAMES 60

/* Step of the Super CO Power cut-in script, after SuperBreak_LettersPopInUpdate:
 * keeps drawing the power name letters at normal size for HOLD_FRAMES frames, then
 * resets the frame counter and resumes the script. */
void SuperBreak_HoldName(Task *task)
{
    int i;

    FRAME(task)++;
    func_02002620(184, 160, 0x800, 16, 2, 0);
    for (i = 0; i < task->unk_58; i++) {
        func_0202a2c4(i, func_0202a39c(i) & 0x1ff, 80, 0, 1);
    }
    if (FRAME(task) == HOLD_FRAMES) {
        FRAME(task) = 0;
        Task_ResumeScript(task);
    }
}
