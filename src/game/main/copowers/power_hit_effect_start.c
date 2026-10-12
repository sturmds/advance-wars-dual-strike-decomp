#include "types.h"
#include "game/power_hit_effect_task.h"
#include "game/task.h"
#include "game/Unk_02080440.h"

#define STATE (data_027c027c->unk_48[data_027c0284])

void func_020f5888(s16 x, s16 y);

extern TaskScriptCmd data_021688dc[];
extern Unk_02080440* data_027c027c;
extern int data_027c0284;

/* Shows the effect of a power on a tile for `player`. A tile that is not
 * shown (MapTiles.unk_2338 is 0) only gets func_020f5888, which starts the
 * map scroll to it; otherwise a task plays the effect once the scroll is
 * done. */
void PowerHitEffect_Start(int x, int y, int player) {
  PowerHitEffectTask* task;

  if (STATE->mapTiles->unk_2338[STATE->mapTiles->rowStart[y] + x] == 0) {
    func_020f5888(x, y);
    return;
  }
  task = (PowerHitEffectTask*)Task_Create(data_021688dc, TASK_LIST(3));
  task->tileX = x;
  task->tileY = y;
  task->player = player;
}
