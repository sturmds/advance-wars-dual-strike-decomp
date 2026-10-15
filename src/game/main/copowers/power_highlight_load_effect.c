#include "types.h"
#include "game/co_data.h"
#include "game/player.h"
#include "game/Unk_02080440.h"

void PowerHighlight_LoadGraphics(int effect, int color, int a, int b);

#define FRONT(player) (MAP_STATE->players[player].front)

/* Loads the unit highlight effect of the player's front CO block, picked by
 * unk_12 like PowerHighlightTask_Update does (highlightEffect and
 * highlightColor go to PowerHighlight_LoadGraphics, which loads graphics files).
 * Needed to match: the block is indexed in both arguments. */
void PowerHighlight_LoadEffect(int player) {
  PowerHighlight_LoadGraphics(gCoTable[FRONT(player).id & PLAYER_CO_ID_MASK]
                                  .blocks[FRONT(player).unk_12]
                                  .highlightEffect,
                              gCoTable[FRONT(player).id & PLAYER_CO_ID_MASK]
                                  .blocks[FRONT(player).unk_12]
                                  .highlightColor,
                              404, 5);
}
