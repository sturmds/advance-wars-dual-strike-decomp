#include "types.h"
#include "game/task.h"
#include "game/Unk_02080440.h"
#include "game/play_records.h"
#include "game/player.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern const TaskScriptCmd data_021684f8[];
extern const TaskScriptCmd data_02168578[];

void Player_EndPower(int player);
void Player_SwapCos(int player);

#define GAME (data_027c027c->unk_48[data_027c0284])

void UnitAction_SwapCo(void) {
  Player_EndPower(GAME->currentPlayer);
  switch (GAME->players[GAME->currentPlayer].tagPowerHalf) {
    case 0:
      if (GAME->players[GAME->currentPlayer].controller ==
          PLAYER_CONTROLLER_HUMAN) {
        if (gPlayRecords.coSwaps < 999999) {
          gPlayRecords.coSwaps++;
        }
      }
      Player_SwapCos(GAME->currentPlayer);
      Task_Create(data_021684f8, TASK_LIST(3));
      break;
    case TAG_POWER_FRONT:
      if (GAME->players[GAME->currentPlayer].controller ==
          PLAYER_CONTROLLER_HUMAN) {
        if (gPlayRecords.coSwaps < 999999) {
          gPlayRecords.coSwaps++;
        }
      }
      GAME->players[GAME->currentPlayer].tagPowerHalf = TAG_POWER_BACK;
      Player_SwapCos(GAME->currentPlayer);
      Task_Create(data_02168578, TASK_LIST(3));
      break;
    case TAG_POWER_BACK:
      break;
  }
}
