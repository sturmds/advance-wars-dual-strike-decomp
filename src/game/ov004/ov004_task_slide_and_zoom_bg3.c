#include "types.h"
#include "game/Unk_ov004_022bf158.h"

extern Unk_ov004_022bf158 *data_ov004_022bf158;

int Math_Ease(int curve, int from, int to, int elapsed, int duration);
int Math_EaseOvershoot(int param, int from, int to, int elapsed, int duration);
void Task_ResumeScript(void *task);

void Ov004Task_SlideAndZoomBg3(Ov004Task *task) {
    Unk_ov004_022bf158 *state = data_ov004_022bf158;
    u16 scale;

    switch (task->step) {
    case 0:
        task->frame = 0;
        task->duration = task->tweenFrames;
        task->step++;
    case 1:
        Ov004_SetBg3Center(state, Math_EaseOvershoot(110, task->tweenFrom, task->tweenTo, task->frame, task->duration), 96);
        scale = Math_Ease(0, 304, 256, task->frame, task->duration);
        Ov004_SetBg3Scale(state, scale, scale);
        if (task->frame <= task->duration) {
            task->frame++;
        }
        if (task->frame > task->duration) {
            task->step++;
        }
        break;
    case 2:
        task->frame = 0;
        task->duration = task->unk_48;
        task->step++;
    case 3:
        Ov004_SetBg3Center(state, Math_Ease(0, task->tweenTo, task->unk_5c, task->frame, task->duration), 96);
        if (task->frame <= task->duration) {
            task->frame++;
        }
        if (task->frame > task->duration) {
            Task_ResumeScript(task);
        }
        break;
    }
}
