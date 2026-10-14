#include "types.h"
#include "game/battle_scene.h"
#include "game/menu_state.h"

extern MenuState gMenuState;
s16 data_ov002_023604dc[][3] = {
#include "data/ov002/data_ov002_023604dc.inc.c"
};
volatile s32 data_ov002_023604f4[] = {
#include "data/ov002/data_ov002_023604f4.inc.c"
};

int Math_Ease(int curve, int from, int to, int elapsed, int duration);
void Task_ResumeScript(void* task);

/* Eases both sides' sway to 0 over 50 frames, then resumes the task's script. */
void Battle_UpdateSway(BattleAnimTask* task) {
  int i;

  for (i = 0; i < 2; i++) {
    gMenuState.battle.sway[i].x = Math_Ease(
        4, data_ov002_023604dc[(i == 0) ? task->phase[0] : task->phase[1]][0],
        0, task->progress, 50);
    gMenuState.battle.sway[i].y = Math_Ease(
        4, data_ov002_023604dc[(i == 0) ? task->phase[0] : task->phase[1]][1],
        0, task->progress, 50);
    gMenuState.battle.sway[i].z = Math_Ease(
        4, data_ov002_023604dc[(i == 0) ? task->phase[0] : task->phase[1]][2],
        0, task->progress, 50);
    gMenuState.battle.unk_40c[i] = gMenuState.battle.sway[0].x >> 4;
  }
  if (task->progress != 50) {
    task->progress++;
  } else {
    Task_ResumeScript(task);
  }
}
