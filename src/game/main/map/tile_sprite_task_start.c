#include "types.h"
#include "game/map_header.h"
#include "game/task.h"
#include "game/Unk_02080440.h"
#include "game/tile_sprite_task.h"
#include "game/design_map.h"

void Sound_PlaySe(int id);

/* One CO's row of data_021536ec, 0x220 bytes: for each kind (0-3), 64 u16
 * with the sound effect ids in its first two entries, picked by the
 * counter. The last 0x20 bytes are not read here. */
typedef struct CoSoundRow {
  u16 byKind[4][64];
  u16 unk_200[16];
} CoSoundRow;

extern const TaskScriptCmd data_02166ed4[];
extern const TaskScriptCmd data_02166efc[];
extern CoSoundRow data_021536ec[];
int data_022a6c78;
u8 gSavedArmyColours[ARMY_SLOT_COUNT];
DesignMap data_022a6c84[0xc]; /* size: up to the next symbol */
extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define GAME (data_027c027c->unk_48[data_027c0284])

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
  Sound_PlaySe(data_021536ec[GAME->players[GAME->currentPlayer].front.id &
                             PLAYER_CO_ID_MASK]
                   .byKind[kind][data_022a6c78]);
  task->x = x;
  task->y = y;
  task->tile = tile;
  task->timer = 0;
  task->unk_30 = 0;
}
