#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/play_records.h"
#include "game/screen_other_map.h"

extern Unk_02080440_48 data_02183b08;
extern Unk_02080440_48 data_02189cd8;
extern PlayRecords gPlayRecords;

int Players_AnyHumanInGame(void);
void Turn_IncrementDay(void);
void CurrentPlayer_SyncState(void);
void CurrentPlayer_LoadPalettes(void);
void func_020c4018(void);
BOOL SceneMode_IsCurrent(int arg);
BOOL Player_IsInGame(int player);

#define GAME (data_027c027c->unk_48[data_027c0284])
#define OWN_MAP ((&data_027c0288.unk_00)[data_027c0284])
#define OTHER_MAP ((&data_027c0288.unk_00)[1 - data_027c0284])

#define SWITCH_TO(state)            \
    saved0280 = data_027c0280;      \
    Screen_SetCurrentGfx(state);    \
    saved027c = data_027c027c;      \
    Screen_SetCurrent(state)

#define SWITCH_BACK()               \
    Screen_SetCurrent(saved027c);   \
    Screen_SetCurrentGfx(saved0280)

/* Passes the turn to the next player still in the game. When the order wraps
 * from player 4 to 1, gPlayRecords.unk_380 goes up by one if tally is set
 * and this is the first state. In a two-map battle whose other map is not
 * over, the other state's currentPlayer is then set to this player and its
 * per-turn setup runs.
 *
 * tally: Players_AnyHumanInGame, TRUE while a human still plays (reused below for the
 * player of this map's state).
 * firstState: 1 when the state is data_02183b08, or data_02189cd8 once the
 * other map is over (reused below for the other state's player). */
void Turn_AdvanceToNextPlayer(void)
{
    int tally;
    int firstState;
    Unk_027c0280 *saved0280;
    Unk_02080440 *saved027c;
    u32 otherUnk30;

    tally = Players_AnyHumanInGame();
    firstState = 0;
    if (GAME->secondMapId != 0) {
        SWITCH_TO(OTHER_MAP);
        otherUnk30 = GAME->unk_1c4->unk_30;
        SWITCH_BACK();
        if (otherUnk30 == 0) {
            if (GAME == &data_02189cd8) {
                firstState = 1;
            }
            goto next;
        }
    }
    if (GAME == &data_02183b08) {
        firstState = 1;
    }
next:
    do {
        GAME->currentPlayer++;
        if (GAME->currentPlayer == 5) {
            Turn_IncrementDay();
            GAME->currentPlayer = 1;
            if (tally != 0 && firstState != 0 && gPlayRecords.unk_380 < 999999) {
                gPlayRecords.unk_380++;
            }
        }
    } while (!Player_IsInGame(GAME->currentPlayer & 0xff));
    if (GAME->secondMapId == 0) {
        return;
    }
    SWITCH_TO(OTHER_MAP);
    otherUnk30 = GAME->unk_1c4->unk_30;
    SWITCH_BACK();
    if (otherUnk30 != 0) {
        return;
    }
    SWITCH_TO(OWN_MAP);
    tally = GAME->currentPlayer;
    SWITCH_BACK();
    SWITCH_TO(OTHER_MAP);
    firstState = GAME->currentPlayer;
    SWITCH_BACK();
    if (GAME == &data_02183b08) {
        SWITCH_TO(OTHER_MAP);
        GAME->currentPlayer = tally;
        CurrentPlayer_SyncState();
        if (data_027c027c != data_027c0288.unk_04 || SceneMode_IsCurrent(4)) {
            CurrentPlayer_LoadPalettes();
            func_020c4018();
        }
        SWITCH_BACK();
    }
    if (GAME == &data_02189cd8) {
        SWITCH_TO(OWN_MAP);
        GAME->currentPlayer = firstState;
        CurrentPlayer_SyncState();
        if (data_027c027c != data_027c0288.unk_04 || SceneMode_IsCurrent(4)) {
            CurrentPlayer_LoadPalettes();
            func_020c4018();
        }
        SWITCH_BACK();
    }
}
