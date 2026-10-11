#include "types.h"
#include "game/Unk_ov004_022bf158.h"

extern Unk_ov004_022bf158 *data_ov004_022bf158;

int Math_Ease(int curve, int from, int to, int elapsed, int duration);
int Math_EaseOvershoot(int amount, int from, int to, int elapsed, int duration);
void Task_ResumeScript(void *task);

void Ov004_EntryAppear(Ov004Task *task) {
    Unk_ov004_022bf158 *state = data_ov004_022bf158;

    switch (task->step) {
    case 0:                            /* falls through into the first frame */
        task->frame = 0;
        task->duration = 7;
        task->step++;
    case 1:
        state->entries[task->tweenFrames].active = 1;
        state->entries[task->tweenFrames].shade = Math_Ease(0, 31, 0, task->frame, task->duration);
        state->entries[task->tweenFrames].width = Math_EaseOvershoot(100, 2, 64, task->frame, task->duration);
        state->entries[task->tweenFrames].height = Math_EaseOvershoot(100, 190, 128, task->frame, task->duration);
        if (task->frame <= task->duration) {
            task->frame++;
        }
        if (task->frame > task->duration) {
            Task_ResumeScript(task);
        }
        break;
    }
}
