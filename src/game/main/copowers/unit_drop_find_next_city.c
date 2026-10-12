#include "types.h"
#include "game/task.h"
#include "game/UnitDropTask.h"
#include "game/Unk_02080440.h"
#include "game/terrain.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

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

  if (Unit_FindFreeSlot(STATE->currentPlayer) == NULL) {
    Task_JumpToLabel((Task*)task, UNIT_DROP_LABEL_DONE);
    return;
  }
  for (; task->y < STATE->unk_1c4->mapHeight; task->y++) {
    for (; task->x < STATE->unk_1c4->mapWidth; task->x++) {
      tile = task->x + STATE->mapTiles->rowStart[task->y];
      if (MAP_TILE(TERRAIN_CITY, STATE->currentPlayer) ==
              STATE->mapTiles->tiles[tile] &&
          STATE->mapTiles->unk_508[tile] == 0) {
        func_020f5888(task->x, task->y);
        return;
      }
    }
    task->x = 0;
  }
  Task_JumpToLabel((Task*)task, UNIT_DROP_LABEL_DONE);
}
