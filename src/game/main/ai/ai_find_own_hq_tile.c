#include "types.h"
#include "game/Unk_02080440.h"
#include "game/player.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

BOOL Player_HasHq(int player);
void RangeMap_FillMoves(int x, int y, int unitType, int movement, int arg4);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define PLAYER (STATE->players[STATE->currentPlayer])

#define FAR_AWAY 110
#define HQ_COORD_MASK \
  0x7f /* the low 7 bits of Player.hqX and Player.hqY hold the HQ tile; bit 0x80 of hqX means no HQ */

/* Writes the x and y of the current player's HQ to out when the acting
 * unit (not a sea unit) can reach it. */
BOOL Ai_FindOwnHqTile(s16* out) {
  if (Ov000_gUnitStats[STATE->actingUnit->type].moveGroup & MOVE_GROUP_SEA) {
    return FALSE;
  }
  if (!Player_HasHq(STATE->currentPlayer)) {
    return FALSE;
  }
  RangeMap_FillMoves(STATE->actingUnit->x, STATE->actingUnit->y,
                     STATE->actingUnit->type, FAR_AWAY, 0);
  if (STATE->rangeMap[PLAYER.hqY & HQ_COORD_MASK][PLAYER.hqX & HQ_COORD_MASK] <
      0) {
    return FALSE;
  }
  out[0] = PLAYER.hqX & HQ_COORD_MASK;
  out[1] = PLAYER.hqY & HQ_COORD_MASK;
  return TRUE;
}
