#include "types.h"
#include "game/Unk_02080440.h"
#include "game/range_map.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

BOOL Player_IsInGame(u8 player);
void func_020973fc(u8 player);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Clears MapTiles.rangeScratch, then lets func_020973fc add 1 there for each
 * enemy Sub that can reach or get next to the tile. A Sub with a Cruiser of
 * the current player's team inside that reach is skipped. */
void Ai_CountEnemySubReach(void)
{
    u16 savedUnk1e;
    int player;

    savedUnk1e = STATE->unk_1e;
    RangeMap_SetBuffer(STATE->mapTiles->rangeScratch);
    RangeMap_Fill(0);
    RangeMap_SetBuffer(STATE->mapTiles->rangeMapTiles);
    for (player = 1; player < 5; player++) {
        if (Player_IsInGame(player) && STATE->players[STATE->currentPlayer].team != STATE->players[player].team) {
            func_020973fc(player);
        }
    }
    STATE->unk_1e = savedUnk1e;
}
