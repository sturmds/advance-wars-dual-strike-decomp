#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_object.h"

MapObject* Map_FindObjectOfKindCovering(int x, int y, int kind);
MapUnit* Unit_FindFreeSlot(int player);

/* TRUE when the Oozium of Ov001_SpawnOoziumAt3_10 can appear: a kind 11
 * object covers tile (3, 9), the unit on tile (3, 10) (if any) does not
 * belong to the current player, and the current player has a free unit slot. */
BOOL Ov001_CanSpawnOoziumAt3_10(void) {
  u8 slot;

  if (Map_FindObjectOfKindCovering(3, 9, 11) == NULL) {
    return FALSE;
  }
  slot = MAP_STATE->mapTiles->unk_508[3 + MAP_STATE->mapTiles->rowStart[10]];
  if (slot != 0 && (slot & 0xc0) == MAP_STATE->currentUnitBase) {
    return FALSE;
  }
  if (Unit_FindFreeSlot(MAP_STATE->currentPlayer) == NULL) {
    return FALSE;
  }
  return TRUE;
}
