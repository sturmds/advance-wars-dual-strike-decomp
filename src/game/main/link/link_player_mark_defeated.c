#include "types.h"
#include "game/Unk_02080440.h"
#include "game/player.h"
#include "game/play_records.h"

#define RECORD_MAX 999999

/* Defeat codes (see func_020cef2c): 1 and 16 no units left, 2 failed the
 * Player.unk_34 check. */
#define DEFEAT_NO_UNITS_MASK 0x11
#define DEFEAT_CODE_2 2

/* Marks player out of the battle: Player.defeated gets the current day and
 * Player.defeatCode the defeat code. Then counts the defeat in gPlayRecords: a
 * human player beaten outside Survival, and a defeat of another player on a
 * human player's turn. */
void LinkPlayer_MarkDefeated(int player, int code) {
  MAP_STATE->players[player].defeated = MAP_STATE->day;
  MAP_STATE->players[player].defeatCode = code;
  if (MAP_STATE->gameMode != GAME_MODE_SURVIVAL &&
      MAP_STATE->players[player].controller == PLAYER_CONTROLLER_HUMAN) {
    if (gPlayRecords.unk_410 < RECORD_MAX) {
      gPlayRecords.unk_410++;
    }
  }
  if (player == MAP_STATE->currentPlayer ||
      MAP_STATE->players[MAP_STATE->currentPlayer].controller !=
          PLAYER_CONTROLLER_HUMAN) {
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
