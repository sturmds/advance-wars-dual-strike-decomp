#include "types.h"
#include "game/ai_unit_action.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

int AiTransport_FindDropTiles(int x, int y, s16* firstDrop, s16* secondDrop);
BOOL AiTransport_SecondCargoHasTaskAt(int x, int y);
int Tile_GetDirection(
    int fromX, int fromY, int toX,
    int toY); /* direction: 0 same tile, 1 up, 2 right, 3 down, 4 left */
void AiUnit_PerformAction(s16 x, s16 y, int action, u8 arg3, u8 arg4);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* MapUnit.aiOrder values copied between cargo units: 0-6. Orders 8-10 use
 * the unit's aiTarget; 7 has no routine in AiTransport_SecondCargoHasTaskAt. */
#define AI_ORDER_COPY_LIMIT 7

/* Queues the acting transport's unload at (x, y), on the tiles
 * AiTransport_FindDropTiles picks. When both cargo units have a drop tile
 * and the second has a task there, an unload with both directions is
 * queued first. When the second has no task there and neither unit is a
 * foot unit or an APC, the second takes the first one's order. The unload
 * of the first unit alone is queued in every case. */
void AiTransport_UnloadAt(int x, int y) {
  s16 firstDrop[2];
  s16 secondDrop[2];
  int dropCount;
  int firstDir;
  MapUnit* first;
  MapUnit* second;

  dropCount = AiTransport_FindDropTiles(x, y, firstDrop, secondDrop);
  if (dropCount == 0) {
    return;
  }
  firstDir = Tile_GetDirection(x, y, firstDrop[0], firstDrop[1]);
  if (dropCount >= 2) {
    if (AiTransport_SecondCargoHasTaskAt(x, y) == TRUE) {
      AiUnit_PerformAction(
          x, y, AI_ACTION_UNLOAD, firstDir,
          Tile_GetDirection(x, y, secondDrop[0], secondDrop[1]));
    } else {
      first = &STATE->units[STATE->actingUnit->cargo[0]];
      if (first->type > UNIT_MECH && first->type != UNIT_APC &&
          first->aiOrder < AI_ORDER_COPY_LIMIT) {
        second = &STATE->units[STATE->actingUnit->cargo[1]];
        if (second->type > UNIT_MECH && second->type != UNIT_APC &&
            second->aiOrder < AI_ORDER_COPY_LIMIT) {
          second->aiOrder = first->aiOrder;
        }
      }
    }
  }
  AiUnit_PerformAction(x, y, AI_ACTION_UNLOAD, firstDir, 0);
}
