#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_object.h"

MapObject* Map_FindObjectOfKindCovering(int x, int y, int kind);
MapUnit* Unit_FindFreeSlot(int player);

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* TRUE when the Oozium of Ov001_SpawnOoziumAt9_12 can appear: a kind 12
 * object covers tile (9, 11), the unit on tile (9, 12) (if any) does not
 * belong to the current player, and the current player has a free unit slot. */
BOOL Ov001_CanSpawnOoziumAt9_12(void) {
  u8 slot;

  if (Map_FindObjectOfKindCovering(9, 11, 12) == NULL) {
    return FALSE;
  }
  slot = STATE->mapTiles->unk_508[9 + STATE->mapTiles->rowStart[12]];
  if (slot != 0 && (slot & 0xc0) == STATE->currentUnitBase) {
    return FALSE;
  }
  if (Unit_FindFreeSlot(STATE->currentPlayer) == NULL) {
    return FALSE;
  }
  return TRUE;
}
