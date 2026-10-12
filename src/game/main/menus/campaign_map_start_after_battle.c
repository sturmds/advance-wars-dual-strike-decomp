#include "types.h"
#include "game/campaign_map.h"
#include "game/task.h"

extern TaskScriptCmd data_0215b5e8[];

/* Starts the world map as a child of parent with mode 2, the mode that
 * CampaignMap_Init treats as returning from a battle (it marks the mission
 * done). */
void CampaignMap_StartAfterBattle(Task* parent) {
  Unk_0204f960* task = (Unk_0204f960*)Task_CreateChild(data_0215b5e8, parent);

  task->unk_52 = 2;
}
