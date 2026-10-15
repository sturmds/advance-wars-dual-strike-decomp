#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_object.h"
#include "game/object_attack_task.h"
#include "game/play_records.h"
#include "game/player.h"
#include "game/task.h"

/* needed to match: the original loads data_027c0284 before data_027c027c
 * here (the header's STATE_IDX_FIRST loads the base first). */
#define STATE_INDEX_LOADED_FIRST                                          \
  (*(Unk_02080440_48**)((u8*)(data_027c0284 * sizeof(Unk_02080440_48*)) + \
                            (u32)data_027c027c + (u32) &                  \
                        ((Unk_02080440*)0)->unk_48))
#define DESTROYED_COUNT(player, index) \
  (STATE_INDEX_LOADED_FIRST->players[player].destroyedCounts[index])

#define TILE_HP_MASK 0x7f

void MapObjectHitTask_Start(MapObject* object, Task* parent);
void PipeSeamHitTask_Start(int x, int y, Task* parent);

/* Adds 1 (up to 99) to the attacker's count in Player.destroyedCounts[index]. */
#define COUNT_PLAYER(player, index)             \
  {                                             \
    int n = DESTROYED_COUNT(player, index) + 1; \
    if (n >= 99) {                              \
      DESTROYED_COUNT(player, index) = 99;      \
    } else {                                    \
      DESTROYED_COUNT(player, index) = n;       \
    }                                           \
  }

/* Adds 1 (below 1000000) to a play record when the attacker is human. */
#define COUNT_RECORD(player, field)                                       \
  if (MAP_STATE->players[player].controller == PLAYER_CONTROLLER_HUMAN) { \
    if (gPlayRecords.field < 999999) {                                    \
      gPlayRecords.field++;                                               \
    }                                                                     \
  }

/* Starts the hit display for the object or pipe seam the attack task hit.
 * When that destroyed it, counts it for the attacker's player (Player.destroyedCounts:
 * 0 Black Cannons, 1 Minicannons, 2 pipe seams, 4 Black Crystals; Black
 * Obelisks get no count) and, for a human player, in gPlayRecords. */
void ObjectAttack_ShowHit(ObjectAttackTask* task) {
  MapObject* object;
  int player;
  int hp;

  object = task->object;
  player = MAP_UNIT_OWNER(MAP_STATE, MAP_STATE->combatants[0].unit);
  if (object != NULL) {
    MapObjectHitTask_Start(object, (Task*)task);
    hp = object->hp;
  } else {
    PipeSeamHitTask_Start(task->tileX, task->tileY, (Task*)task);
    hp =
        MAP_STATE
            ->tileHp[task->tileX + MAP_STATE->mapTiles->rowStart[task->tileY]] &
        TILE_HP_MASK;
  }
  if (hp != 0) {
    return;
  }

  if (object != NULL) {
    switch (object->kind) {
      case MAP_OBJECT_BLACK_CRYSTAL:
        COUNT_PLAYER(player, 4);
        COUNT_RECORD(player, blackCrystalsDestroyed);
        break;
      case MAP_OBJECT_MINICANNON:
        COUNT_PLAYER(player, 1);
        COUNT_RECORD(player, minicannonsDestroyed);
        break;
      case MAP_OBJECT_BLACK_CANNON:
        COUNT_PLAYER(player, 0);
        COUNT_RECORD(player, blackCannonsDestroyed);
        break;
      case MAP_OBJECT_BLACK_OBELISK:
        COUNT_RECORD(player, blackObelisksDestroyed);
        break;
      case 0:
        COUNT_PLAYER(player, 2);
        COUNT_RECORD(player, pipeSeamsDestroyed);
        break;
    }
  } else {
    COUNT_PLAYER(player, 2);
    COUNT_RECORD(player, pipeSeamsDestroyed);
  }
}
