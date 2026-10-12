#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

BOOL Map_IsSiloOrCapturableTile(int x, int y);
BOOL Unit_IsCurrentPlayers(s16 unitSlot);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Path search callback: TRUE for a silo or capturable tile that is empty,
 * or holds one of the current player's units that is not Infantry or Mech. */
int Ai_IsFreeCaptureSiteTile(int x, int y, int arg2) {
  int slot;

  if (!Map_IsSiloOrCapturableTile(x, y)) {
    return FALSE;
  }
  slot = STATE->mapTiles->unitAt[x + STATE->mapTiles->rowStart[y]];
  if (slot == 0) {
    return TRUE;
  }
  if (!Unit_IsCurrentPlayers(slot)) {
    return FALSE;
  }
  if (STATE->units[slot].type > UNIT_MECH) {
    return TRUE;
  }
  return FALSE;
}
