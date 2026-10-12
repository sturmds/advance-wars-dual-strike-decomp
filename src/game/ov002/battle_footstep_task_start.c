#include "types.h"
#include "game/task.h"
#include "game/battle_scene.h"
#include "game/terrain.h"
#include "game/unit_stats.h"
#include "game/BattleFootstepTask.h"

extern const TaskScriptCmd data_ov002_02369790[];

/* The unit type of the side at byte offset `offset` (side * sizeof(BattleSide)),
 * as BATTLE_SIDE_UNIT_TYPE reads it. */
#define UNIT_TYPE_AT(offset) (((u8*)Ov002_gBattleScene + (offset))[3])

#define SOUND(group, variant) (((group) << 16) | (variant))

/* Needed to match: the original has one copy of this switch for each group,
 * with the store and the return in every case. */
#define PICK_SOUND(group)                                                      \
  switch (((BattleSide*)((u8*)Ov002_gBattleScene->sides + offset))->terrain) { \
    case TERRAIN_MOUNTAIN:                                                     \
      task->sound = SOUND(group, 5);                                           \
      return;                                                                  \
    case TERRAIN_WOOD:                                                         \
      task->sound = SOUND(group, 2);                                           \
      return;                                                                  \
    case TERRAIN_ROAD:                                                         \
    case TERRAIN_CITY:                                                         \
    case TERRAIN_HQ:                                                           \
    case TERRAIN_AIRPORT:                                                      \
    case TERRAIN_PORT:                                                         \
    case TERRAIN_BRIDGE:                                                       \
    case TERRAIN_BASE:                                                         \
    case TERRAIN_SILO:                                                         \
    case TERRAIN_SILO_USED:                                                    \
    case TERRAIN_LAB:                                                          \
    case TERRAIN_COM_TOWER:                                                    \
      task->sound = SOUND(group, 1);                                           \
      return;                                                                  \
    case TERRAIN_RIVER:                                                        \
    case TERRAIN_SEA:                                                          \
    case TERRAIN_REEF:                                                         \
      task->sound = SOUND(group, 3);                                           \
      return;                                                                  \
    case TERRAIN_SHOAL:                                                        \
      task->sound = SOUND(group, 6);                                           \
      return;                                                                  \
  }                                                                            \
  switch (Ov002_gBattleScene->tileset) {                                       \
    case 2:                                                                    \
      task->sound = SOUND(group, 6);                                           \
      return;                                                                  \
    case 3:                                                                    \
      task->sound = SOUND(group, 7);                                           \
      return;                                                                  \
    case 1:                                                                    \
      task->sound = SOUND(group, 4);                                           \
      return;                                                                  \
    default:                                                                   \
      task->sound = SOUND(group, 0);                                           \
      return;                                                                  \
  }

/* Starts the sound task of a foot unit (infantry, or type 27): the sound
 * depends on the unit's group (3 for these two, 4 for other foot units) and
 * on the terrain it stands on, else on the battle's tileset. Probably the
 * marching sound; the task script stops the sound effect when it ends.
 * Needed to match: side * 12 kept in `offset`, and the unit type and the
 * terrain read from two different bases so the sums are not shared. */
void BattleFootstepTask_Start(int side) {
  BattleFootstepTask* task =
      (BattleFootstepTask*)Task_Create(data_ov002_02369790, TASK_LIST(2));
  int offset;

  task->unk_4c = 0;
  task->unk_4e = 0;
  offset = side * sizeof(BattleSide);
  task->side = side;
  if (UNIT_TYPE_AT(offset) == UNIT_INFANTRY || UNIT_TYPE_AT(offset) == 27) {
    PICK_SOUND(3)
  } else {
    PICK_SOUND(4)
  }
}
