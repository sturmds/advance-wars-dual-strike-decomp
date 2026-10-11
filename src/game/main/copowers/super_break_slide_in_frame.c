#include "types.h"
#include "game/task.h"
#include "game/screen_other_map.h"

int Math_Ease(int curve, int from, int to, int elapsed, int duration);
void Sprite_QueueCoPortrait(int x, int y, int arg2, int arg3, int list, BOOL flip);
void Task_ResumeScript(Task *task);

/* The frame counter shares its slot with Task.unk_4c. */
#define FRAME(task) (*(s16 *)&(task)->unk_4c)
/* Slide offset kept in the task's free bytes at 0x2c. */
#define SLIDE(task) (*(int *)(task)->pad_2c)

#define EASE_FRAMES 48

/* Step of the Super CO Power cut-in script. Over EASE_FRAMES frames: SLIDE eases from
 * 128 to 0 (what Sprite_QueueCoPortrait draws moves in from the right), EVA 0 to 16 and EVB
 * 16 to 0 (the blended layer fades fully in). Then it resumes the script. */
void SuperBreak_SlideInFrame(Task *task)
{
    FRAME(task)++;
    SLIDE(task) = Math_Ease(4, 128, 0, FRAME(task), EASE_FRAMES);
    data_027c0280->blendEva = Math_Ease(0, 0, 16, FRAME(task), EASE_FRAMES);
    data_027c0280->blendEvb = Math_Ease(0, 16, 0, FRAME(task), EASE_FRAMES);
    Sprite_QueueCoPortrait((184 - SLIDE(task)) & 0x1ff, 160, 0x800, 16, 2, 0);
    if (FRAME(task) == EASE_FRAMES) {
        FRAME(task) = 0;
        Task_ResumeScript(task);
    }
}
