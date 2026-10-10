#include "types.h"
#include "game/menu_state.h"

typedef struct SwayTask {
    u8 pad_00[0x4c];
    s16 frame;
    u8 pad_4e[0x64 - 0x4e];
    s16 row[2];                        /* row of data_ov002_023604dc each screen starts from */
} SwayTask;

extern MenuState gMenuState;
extern MenuStateFrom200 data_0229ed30;
extern s16 data_ov002_023604dc[][3];

int Math_Ease(int curve, int from, int to, int elapsed, int duration);
void Task_ResumeScript(void *task);

void func_ov002_02353254(SwayTask *task)
{
    int i;

    for (i = 0; i < 2; i++) {
        gMenuState.battle.sway[i].x = Math_Ease(4, data_ov002_023604dc[(i == 0) ? task->row[0] : task->row[1]][0], 0, task->frame, 50);
        gMenuState.battle.sway[i].y = Math_Ease(4, data_ov002_023604dc[(i == 0) ? task->row[0] : task->row[1]][1], 0, task->frame, 50);
        gMenuState.battle.sway[i].z = Math_Ease(4, data_ov002_023604dc[(i == 0) ? task->row[0] : task->row[1]][2], 0, task->frame, 50);
        gMenuState.battle.unk_40c[i] = data_0229ed30.sway0x >> 4;
    }
    if (task->frame != 50) {
        task->frame++;
    } else {
        Task_ResumeScript(task);
    }
}
