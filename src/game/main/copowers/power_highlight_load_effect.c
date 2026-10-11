#include "types.h"
#include "game/co_data.h"
#include "game/player.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

void func_020d5c44(int effect, int color, int a, int b);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define FRONT(player) (STATE->players[player].front)

/* Loads the unit highlight effect of the player's front CO block, picked by
 * unk_12 like PowerHighlightTask_Update does (highlightEffect and
 * highlightColor go to func_020d5c44, which loads graphics files).
 * Needed to match: the block is indexed in both arguments. */
void PowerHighlight_LoadEffect(int player)
{
    func_020d5c44(gCoTable[FRONT(player).id & PLAYER_CO_ID_MASK].blocks[FRONT(player).unk_12].highlightEffect,
                  gCoTable[FRONT(player).id & PLAYER_CO_ID_MASK].blocks[FRONT(player).unk_12].highlightColor, 404, 5);
}
