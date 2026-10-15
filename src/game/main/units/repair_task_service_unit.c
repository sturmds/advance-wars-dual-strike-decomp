#include "types.h"
#include "game/Unk_02080440.h"
#include "game/repair_task.h"

int Unit_Rearm(MapUnit* unit, BOOL payCost);
int Unit_Refuel(MapUnit* unit, BOOL payCost);
int Unit_Repair(MapUnit* unit, s16 steps, BOOL payCost);
BOOL Tile_IsOnScreen(
    int x, int y); /* TRUE when tile (x, y) is in the visible part of the map */
void ServiceCostPopup_Start(s16* pos, int player, int amount, u32 chargeFunds,
                            int zoomVariant, int visible);

/* Step of the repair task script: the repaired unit gets ammo and fuel for
 * free and one shown HP that its owner pays for. When the tile has a
 * nonzero MapTiles.unk_2338 byte and is on screen, ServiceCostPopup_Start gets the
 * tile and the summed cost. */
void RepairTask_ServiceUnit(RepairTask* task) {
  int cost;
  s16 pos[2];

  cost = 0;
  cost += Unit_Rearm(task->unit, FALSE);
  cost += Unit_Refuel(task->unit, FALSE);
  cost += Unit_Repair(task->unit, 1, TRUE);
  if (MAP_STATE->mapTiles->unk_2338[MAP_TILE_INDEX(task->tileX, task->tileY)] ==
      0) {
    return;
  }
  if (!Tile_IsOnScreen(task->tileX, task->tileY)) {
    return;
  }
  pos[0] = task->tileX;
  pos[1] = task->tileY;
  ServiceCostPopup_Start(pos, (u8)MAP_STATE->currentPlayer, cost, 0, 1, 1);
}
