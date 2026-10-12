#include "types.h"
#include "game/player.h"
#include "game/Unk_02080440.h"
#include "game/Unk_022a7a9c.h"
#include "game/play_records.h"

int Player_GetCurrentOrFirstInGame(void);

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* After a campaign or War Room battle whose current player is human, adds
 * 1 to the play record of the battle's score rank (data_022a7a9c.rank,
 * 2-5). */
void PlayRecords_CountBattleRank(void) {
  BOOL human;

  if (STATE->gameMode != GAME_MODE_CAMPAIGN &&
      STATE->gameMode != GAME_MODE_WAR_ROOM) {
    return;
  }
  /* needed to match: the original turns the test into a BOOL first */
  human = STATE->players[Player_GetCurrentOrFirstInGame()].controller ==
                  PLAYER_CONTROLLER_HUMAN
              ? TRUE
              : FALSE;
  if (!human) {
    return;
  }
  switch (data_022a7a9c.rank) {
    case 5:
      if (gPlayRecords.rank5Battles < 999999) {
        gPlayRecords.rank5Battles++;
      }
      break;
    case 4:
      if (gPlayRecords.rank4Battles < 999999) {
        gPlayRecords.rank4Battles++;
      }
      break;
    case 3:
      if (gPlayRecords.rank3Battles < 999999) {
        gPlayRecords.rank3Battles++;
      }
      break;
    case 2:
      if (gPlayRecords.rank2Battles < 999999) {
        gPlayRecords.rank2Battles++;
      }
      break;
  }
}
