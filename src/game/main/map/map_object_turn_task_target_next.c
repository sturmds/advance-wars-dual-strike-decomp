#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_object.h"
#include "game/map_object_turn_task.h"
#include "game/player.h"
#include "game/tile_point.h"
#include "game/ai.h"

extern TilePointU16* data_02167e98[];

int Task_JumpToLabel(MapObjectTurnTask* task, int label);

/* Step of the map object turn script. Empties the AI target list, then
 * finds the next object that acts this turn and fills the list for it: the
 * object itself first (unit slot 0, its kind as the value), then the units
 * it hits. A volcano acts once its state count is 0 and hits the units on
 * its data_02167e98 tiles (none on day 0). A Black Cannon or Minicannon
 * acts on Black Hole's turn and picks one unit (Cannon_AddBestTarget). The
 * damage comes from the object's unk_07. With no object left, jumps to
 * label 1. */
void MapObjectTurnTask_TargetNext(MapObjectTurnTask* task) {
  MapObject* object;
  u16 kind;

  AiTargetList_Clear();
  while ((kind = task->nextObject->kind) != 0) {
    object = task->nextObject;
    switch (kind) {
      case MAP_OBJECT_VOLCANO:
        if (object->variant != 0 && object->state == 0) {
          AiTargetList_Add(object->x, object->y, 0, kind);
          if (MAP_STATE->day != 0) {
            AiTargetList_AddUnitsOnTiles(
                data_02167e98[task->nextObject->variant],
                (s8)task->nextObject->unk_07);
          }
          task->nextObject++;
          return;
        }
        break;
      case MAP_OBJECT_BLACK_CANNON:
      case MAP_OBJECT_MINICANNON:
        if (MAP_STATE->players[MAP_STATE->currentPlayer].armyColour ==
                ARMY_BLACK_HOLE &&
            object->hp != 0 && object->state == 0) {
          AiTargetList_Add(object->x, object->y, 0, kind);
          object = task->nextObject;
          Cannon_AddBestTarget(object->x, object->y, (s8)object->unk_07,
                               object->kind, object->variant);
          task->nextObject++;
          return;
        }
        break;
    }
    task->nextObject++;
  }
  Task_JumpToLabel(task, 1);
}
