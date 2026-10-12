#include "types.h"
#include "game/campaign_map.h"
#include "game/task.h"

extern TaskScriptCmd data_0215b5e8[];

/* Starts the world map as a child of parent with mode 0 (mode 2 is
 * CampaignMap_StartAfterBattle). */
void CampaignMap_StartMode0(Task* parent) {
  Unk_0204f960* task = (Unk_0204f960*)Task_CreateChild(data_0215b5e8, parent);

  task->unk_52 = 0;
}
