#include "types.h"
#include "game/menu_state.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;
extern MenuStateFrom400 data_0229ef30;

/* Tab of the map select menu: 3 in game mode 1, otherwise 0 for a single
 * map, and 1 or 2 for two maps by setup.firstMapId. */
int MapSelect_GetTab(void)
{
    Unk_02080440_48 *state = data_027c027c->unk_48[data_027c0284];

    if (state->gameMode == GAME_MODE_WAR_ROOM) {
        return 3;
    }
    if (state->twoMaps == 0) {
        return 0;
    }
    if (data_0229ef30.firstMapId == 0) {
        return 1;
    }
    return 2;
}
