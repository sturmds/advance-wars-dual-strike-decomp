#include "types.h"
#include "game/screen_other_map.h"
#include "game/player.h"

extern u8 gSlotControllers[];                 /* per slot: PLAYER_CONTROLLER_*, filled by Players_RecordControllers */
extern u32 data_021696c0[];                /* event script per army colour, for a player in the game */
extern u32 data_02169708[];                /* event script per army colour, for a player out of it */

void Players_RecordControllers(void);
BOOL Player_IsInGame(u8 player);
void EventScript_StartText(u32 script, int arg1, int arg2);

#define GAME (data_027c027c->unk_48[data_027c0284])
#define OTHER_MAP ((&data_027c0288.unk_00)[1 - data_027c0284])

#define SWITCH_TO(state, savedGfx, savedGame) \
    savedGfx = data_027c0280;                 \
    Screen_SetCurrentGfx(state);              \
    savedGame = data_027c027c;                \
    Screen_SetCurrent(state)

#define SWITCH_BACK(savedGfx, savedGame) \
    Screen_SetCurrent(savedGame);        \
    Screen_SetCurrentGfx(savedGfx)

/* Starts an event script picked by army on the other map: the script for a
 * human player still in the game, else the one for a human player who is out,
 * else the one for any player in the game. Each search keeps its last
 * match. Only reached through a pointer in task script data (0x02169944);
 * the name guesses that it runs at the end of a two-map battle. */
void TwoMaps_StartResultScript(void)
{
    Unk_027c0280 *savedGfx;
    Unk_02080440 *savedGame;
    u32 script;
    Unk_027c0280 *lastGfx;                     /* needed to match: own pair for the last search */
    Unk_02080440 *lastGame;
    int i;
    int j;
    int k;

    Players_RecordControllers();
    script = 0;

    SWITCH_TO(OTHER_MAP, savedGfx, savedGame);
    for (i = 1; i < 5; i++) {
        if (Player_IsInGame(i) && gSlotControllers[i] == PLAYER_CONTROLLER_HUMAN) {
            script = data_021696c0[GAME->players[i].armyColour];
        }
    }
    SWITCH_BACK(savedGfx, savedGame);

    if (script == 0) {
        SWITCH_TO(OTHER_MAP, savedGfx, savedGame);
        for (j = 1; j < 5; j++) {
            if (!Player_IsInGame(j) && gSlotControllers[j] == PLAYER_CONTROLLER_HUMAN) {
                script = data_02169708[GAME->players[j].armyColour];
            }
        }
        SWITCH_BACK(savedGfx, savedGame);
    }

    if (script == 0) {
        SWITCH_TO(OTHER_MAP, lastGfx, lastGame);
        for (k = 1; k < 5; k++) {
            if (Player_IsInGame(k)) {
                script = data_021696c0[GAME->players[k].armyColour];
            }
        }
        SWITCH_BACK(lastGfx, lastGame);
    }

    EventScript_StartText(script, 21, 0);
}
