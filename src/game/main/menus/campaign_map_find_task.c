#include "types.h"
#include "game/campaign_map.h"

CampaignMapTask* Task_FindByScript(void* script);

extern u8 data_0215b7b0[];
extern u8 data_0215b720[];
extern u8 data_0215b870[];

CampaignMapTask* CampaignMap_FindTask(void) {
  CampaignMapTask* task = Task_FindByScript(data_0215b7b0);

  if (task != NULL) {
    return task;
  }
  task = Task_FindByScript(data_0215b720);
  if (task != NULL) {
    return task;
  }
  return Task_FindByScript(data_0215b870);
}
