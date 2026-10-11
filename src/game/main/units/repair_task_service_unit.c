#include "types.h"
#include "game/Unk_02080440.h"
#include "game/repair_task.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

int Unit_Rearm(MapUnit *unit, BOOL payCost);
int Unit_Refuel(MapUnit *unit, BOOL payCost);
int Unit_Repair(MapUnit *unit, s16 steps, BOOL payCost);
BOOL Tile_IsOnScreen(int x, int y); /* TRUE when tile (x, y) is in the visible part of the map */
void ServiceCostPopup_Start(s16 *pos, int player, int amount, u32 chargeFunds, int zoomVariant, int visible);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define TILE_INDEX(x, y) ((x) + STATE->mapTiles->rowStart[y])

/* Step of the repair task script: the repaired unit gets ammo and fuel for
 * free and one shown HP that its owner pays for. When the tile has a
 * nonzero MapTiles.unk_2338 byte and is on screen, ServiceCostPopup_Start gets the
 * tile and the summed cost. */
void RepairTask_ServiceUnit(RepairTask *task)
{
    int cost;
    s16 pos[2];

    cost = 0;
    cost += Unit_Rearm(task->unit, FALSE);
    cost += Unit_Refuel(task->unit, FALSE);
    cost += Unit_Repair(task->unit, 1, TRUE);
    if (STATE->mapTiles->unk_2338[TILE_INDEX(task->tileX, task->tileY)] == 0) {
        return;
    }
    if (!Tile_IsOnScreen(task->tileX, task->tileY)) {
        return;
    }
    pos[0] = task->tileX;
    pos[1] = task->tileY;
    ServiceCostPopup_Start(pos, (u8)STATE->currentPlayer, cost, 0, 1, 1);
}
