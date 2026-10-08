#include "types.h"
#include "game/co_data.h"
#include "game/map_header.h"
#include "game/player.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;
extern MapHeader Ov000_gMapHeaders[];
extern u8 data_022a7a68[];             /* COs the player may pick, ends with CO_NONE */

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Returns co if it is in data_022a7a68 and no player (with unk_92 set)
 * has it, either as a pick or from the map header. Otherwise returns the
 * first CO of data_022a7a68 that is still free, or CO_NONE. */
u8 PickFreeCo(int co)
{
    u8 isFree[CO_COUNT];
    int i;
    int player;
    int slot;
    int taken;

    for (i = 0; i < CO_COUNT; i++) {
        isFree[i] = 0;
    }
    for (i = 0; data_022a7a68[i] != CO_NONE; i++) {
        isFree[data_022a7a68[i]] = 1;
    }

    for (player = 1; player < 5; player++) {
        for (slot = 0; slot < 2; slot++) {
            if (STATE->unk_92[player] == 1) {
                taken = STATE->playerCos[player][slot] & PLAYER_CO_ID_MASK;
                if (taken > CO_NONE && taken < CO_COUNT) {
                    isFree[taken] = 0;
                }
                /* MapHeader.co is indexed by player number 1-4 here, so
                 * player 4 reads past co[3]: the header's CO rows start
                 * at +0x56 for player 1. */
                taken = Ov000_gMapHeaders[STATE->unk_1c4->mapId].co[player][slot] & PLAYER_CO_ID_MASK;
                if (taken > CO_NONE && taken < CO_COUNT) {
                    isFree[taken] = 0;
                }
            }
        }
    }

    if (isFree[co]) {
        return co;
    }
    for (i = 0; data_022a7a68[i] != CO_NONE; i++) {
        if (isFree[data_022a7a68[i]]) {
            return data_022a7a68[i];
        }
    }
    return CO_NONE;
}
