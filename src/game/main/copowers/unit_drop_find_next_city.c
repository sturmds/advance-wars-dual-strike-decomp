#include "types.h"
#include "game/task.h"
#include "game/UnitDropTask.h"
#include "game/Unk_02080440.h"
#include "game/terrain.h"

/* Task_JumpToLabel target in gUnitDropScript: the end of the script. */
#define UNIT_DROP_LABEL_DONE 1

MapUnit* Unit_FindFreeSlot(int player);
void Task_JumpToLabel(Task* task, int label);
void func_020f5888(int x, int y);

/* Script step of gUnitDropScript (Sensei's powers): from (task->x, task->y)
 * on, finds the next city the current player owns with no unit on it in
 * MapTiles.unk_508 and scrolls the map there (func_020f5888 starts the map
 * scroll script); the next step drops the unit. Ends the script when the
 * player has no free unit slot or no city is left. */
void UnitDrop_FindNextCity(UnitDropTask* task) {
  int tile;

  if (Unit_FindFreeSlot(MAP_STATE->currentPlayer) == NULL) {
    Task_JumpToLabel((Task*)task, UNIT_DROP_LABEL_DONE);
    return;
  }
  for (; task->y < MAP_STATE->unk_1c4->mapHeight; task->y++) {
    for (; task->x < MAP_STATE->unk_1c4->mapWidth; task->x++) {
      tile = task->x + MAP_STATE->mapTiles->rowStart[task->y];
      if (MAP_TILE(TERRAIN_CITY, MAP_STATE->currentPlayer) ==
              MAP_STATE->mapTiles->tiles[tile] &&
          MAP_STATE->mapTiles->unk_508[tile] == 0) {
        func_020f5888(task->x, task->y);
        return;
      }
    }
    task->x = 0;
  }
  Task_JumpToLabel((Task*)task, UNIT_DROP_LABEL_DONE);
}
