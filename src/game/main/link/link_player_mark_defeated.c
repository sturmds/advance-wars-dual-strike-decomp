#include "types.h"
#include "game/Unk_02080440.h"
#include "game/player.h"
#include "game/play_records.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

#define RECORD_MAX 999999

/* Defeat codes (see func_020cef2c): 1 and 16 no units left, 2 failed the
 * Player.unk_34 check. */
#define DEFEAT_NO_UNITS_MASK 0x11
#define DEFEAT_CODE_2        2

/* Marks player out of the battle: Player.defeated gets the current day and
 * Player.unk_18 the defeat code. Then counts the defeat in gPlayRecords: a
 * human player beaten outside Survival, and a defeat of another player on a
 * human player's turn. */
void LinkPlayer_MarkDefeated(int player, int code)
{
    STATE->players[player].defeated = STATE->day;
    STATE->players[player].unk_18 = code;
    if (STATE->gameMode != GAME_MODE_SURVIVAL && STATE->players[player].controller == PLAYER_CONTROLLER_HUMAN) {
        if (gPlayRecords.unk_410 < RECORD_MAX) {
            gPlayRecords.unk_410++;
        }
    }
    if (player == STATE->currentPlayer
        || STATE->players[STATE->currentPlayer].controller != PLAYER_CONTROLLER_HUMAN) {
        return;
    }
    if (code & DEFEAT_NO_UNITS_MASK) {
        if (gPlayRecords.unk_40c < RECORD_MAX) {
            gPlayRecords.unk_40c++;
        }
    }
    if (code & DEFEAT_CODE_2) {
        if (gPlayRecords.unk_408 < RECORD_MAX) {
            gPlayRecords.unk_408++;
        }
    }
}
