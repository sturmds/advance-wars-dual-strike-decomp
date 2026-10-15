#include "types.h"
#include "game/battle_scene.h"
#include "game/menu_state.h"

extern s16 data_ov002_023604dc[][3];

int MainLoop_GetFrameCount(void);

/* Written as a ternary to match. */
#define PHASE(side) ((side) == 0 ? task->phase[0] : task->phase[1])

/* Starts the ease of the side values: picks a table row per side from the
 * frame counter (low two bits for side 0, the next two for side 1) and loads it. */
void Battle_InitSway(BattleAnimTask* task) {
  int i;

  task->progress = 0;
  task->phase[0] = MainLoop_GetFrameCount() & 3;
  task->phase[1] = (MainLoop_GetFrameCount() >> 2) & 3;
  for (i = 0; i < BATTLE_SIDE_COUNT; i++) {
    gMenuState.battle.sway[i].angle = 0;
    gMenuState.battle.sway[i].x = data_ov002_023604dc[PHASE(i)][0];
    gMenuState.battle.sway[i].y = data_ov002_023604dc[PHASE(i)][1];
    gMenuState.battle.sway[i].z = data_ov002_023604dc[PHASE(i)][2];
  }
}
