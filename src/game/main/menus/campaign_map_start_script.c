#include "types.h"
#include "game/campaign_map.h"
#include "game/task.h"

BOOL Player1Team_HasPlayerInGame(void);

extern TaskScriptCmd data_0215b720[];
extern TaskScriptCmd data_0215b7b0[];
extern TaskScriptCmd data_0215b870[];

/* Starts the task that runs the world map script for the screen task's mode:
 * data_0215b720 for mode 1 (and mode 2 without a player-1 team in the game),
 * data_0215b870 for mode 2 with one, data_0215b7b0 otherwise (the scripts that
 * CampaignMap_FindTask looks for). */
void CampaignMap_StartModeScript(Unk_0204f960* task) {
  switch (task->unk_52) {
    case 1:
      Task_Create(data_0215b720, (Task*)task);
      break;
    case 2:
      if (Player1Team_HasPlayerInGame()) {
        Task_Create(data_0215b870, (Task*)task);
      } else {
        Task_Create(data_0215b720, (Task*)task);
      }
      break;
    default:
      Task_Create(data_0215b7b0, (Task*)task);
      break;
  }
}
