#include "types.h"
#include "game/task.h"
#include "game/Unk_02080440.h"
#include "game/play_records.h"
#include "game/player.h"

extern const TaskScriptCmd data_021684f8[];
extern const TaskScriptCmd data_02168578[];

void Player_EndPower(int player);
void Player_SwapCos(int player);

void UnitAction_SwapCo(void) {
  Player_EndPower(MAP_STATE->currentPlayer);
  switch (MAP_STATE->players[MAP_STATE->currentPlayer].tagPowerHalf) {
    case 0:
      if (MAP_STATE->players[MAP_STATE->currentPlayer].controller ==
          PLAYER_CONTROLLER_HUMAN) {
        if (gPlayRecords.coSwaps < 999999) {
          gPlayRecords.coSwaps++;
        }
      }
      Player_SwapCos(MAP_STATE->currentPlayer);
      Task_Create(data_021684f8, TASK_LIST(3));
      break;
    case TAG_POWER_FRONT:
      if (MAP_STATE->players[MAP_STATE->currentPlayer].controller ==
          PLAYER_CONTROLLER_HUMAN) {
        if (gPlayRecords.coSwaps < 999999) {
          gPlayRecords.coSwaps++;
        }
      }
      MAP_STATE->players[MAP_STATE->currentPlayer].tagPowerHalf =
          TAG_POWER_BACK;
      Player_SwapCos(MAP_STATE->currentPlayer);
      Task_Create(data_02168578, TASK_LIST(3));
      break;
    case TAG_POWER_BACK:
      break;
  }
}
