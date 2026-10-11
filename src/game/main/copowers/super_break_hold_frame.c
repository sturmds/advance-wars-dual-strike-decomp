#include "types.h"
#include "game/task.h"

void Sprite_QueueCoPortrait(int x, int y, int arg2, int arg3, int list, BOOL flip);
void Task_ResumeScript(Task *task);

/* The frame counter shares its slot with Task.unk_4c. */
#define FRAME(task) (*(s16 *)&(task)->unk_4c)
/* Slide offset kept in the task's free bytes at 0x2c. */
#define SLIDE(task) (*(int *)(task)->pad_2c)

#define SLIDE_FRAMES 16

/* Step of the Super CO Power cut-in script: for SLIDE_FRAMES frames draws what
 * Sprite_QueueCoPortrait draws at x = 184 - SLIDE (SLIDE is the offset the earlier steps
 * left in the task), then resets the frame counter and resumes the script. */
void SuperBreak_HoldFrame(Task *task)
{
    FRAME(task)++;
    Sprite_QueueCoPortrait((184 - SLIDE(task)) & 0x1ff, 160, 0x800, 16, 2, 0);
    if (FRAME(task) == SLIDE_FRAMES) {
        FRAME(task) = 0;
        Task_ResumeScript(task);
    }
}
