#include "types.h"
#include "game/co_data.h"
#include "game/map_header.h"
#include "game/task.h"
#include "game/Unk_02080440.h"
#include "game/tile_sprite_task.h"
#include "game/design_map.h"

void Sound_PlaySe(int id);

extern const TaskScriptCmd data_02166ed4[];
extern const TaskScriptCmd data_02166efc[];
int data_022a6c78;
u8 gSavedArmyColours[ARMY_SLOT_COUNT];
DesignMap data_022a6c84[0xc]; /* size: up to the next symbol */

#define MAP_STATE (data_027c027c->unk_48[data_027c0284])

/* No direct callers. Starts a TileSpriteTask from data_02166efc when
 * kind is 2, else from data_02166ed4, and plays a sound effect of the current
 * player's front CO picked by kind and a counter that alternates 0 and 1. */
void TileSpriteTask_Start(u8 x, u8 y, u16 tile, int kind) {
  TileSpriteTask* task;

  data_022a6c78++;
  if (data_022a6c78 > 1) {
    data_022a6c78 = 0;
  }
  if (kind == 2) {
    task = (TileSpriteTask*)Task_Create(data_02166efc, TASK_LIST(3));
  } else {
    task = (TileSpriteTask*)Task_Create(data_02166ed4, TASK_LIST(3));
  }
  /* Needed to match: the sound ids of power block `kind` read as rows of
   * one CoPowerBlock's halfwords from blocks[0].sounds. */
  Sound_PlaySe(((u16(*)[sizeof(CoPowerBlock) / sizeof(u16)]) CO_TABLE_FIELD(
      MAP_STATE->players[MAP_STATE->currentPlayer].front.id & PLAYER_CO_ID_MASK,
      blocks[0].sounds))[kind][data_022a6c78]);
  task->x = x;
  task->y = y;
  task->tile = tile;
  task->timer = 0;
  task->unk_30 = 0;
}
