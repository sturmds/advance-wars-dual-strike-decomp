#include "types.h"
#include "game/Unk_02080440.h"
#include "game/player.h"
#include "game/unit_stats.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;
extern u8 data_ov000_022f4fd7[][sizeof(UnitStats)];

BOOL Player_HasHq(int player);
void RangeMap_FillMoves(int x, int y, int unitType, int movement, int arg4);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define PLAYER (STATE->players[STATE->currentPlayer])

#define FAR_AWAY 110
#define HQ_COORD_MASK 0x7f             /* the low 7 bits of Player.unk_2f (x) and Player.unk_30 (y) hold the HQ tile; bit 0x80 of unk_2f means no HQ */

/* Writes the x and y of the current player's HQ to out when the acting
 * unit (not a sea unit) can reach it. */
BOOL Ai_FindOwnHqTile(s16 *out)
{
    if (data_ov000_022f4fd7[STATE->actingUnit->type][0] & MOVE_GROUP_SEA) {
        return FALSE;
    }
    if (!Player_HasHq(STATE->currentPlayer)) {
        return FALSE;
    }
    RangeMap_FillMoves(STATE->actingUnit->x, STATE->actingUnit->y, STATE->actingUnit->type, FAR_AWAY, 0);
    if (STATE->rangeMap[PLAYER.unk_30 & HQ_COORD_MASK][PLAYER.unk_2f & HQ_COORD_MASK] < 0) {
        return FALSE;
    }
    out[0] = PLAYER.unk_2f & HQ_COORD_MASK;
    out[1] = PLAYER.unk_30 & HQ_COORD_MASK;
    return TRUE;
}
