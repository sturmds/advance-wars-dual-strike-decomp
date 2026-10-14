#include "types.h"
#include "game/screen_contexts.h"
#include "game/task.h"

void CampaignRecapCameraTask_Init(void);
void CampaignRecapTask_Free(void);
void CampaignRecapTask_Init(void);
void CampaignRecap_SetUpScreens(void);
void CampaignRecap_Update(void);
void CampaignRecap_UpdateCamera(void);
extern u8 data_0217eb08[];
extern u8 data_02180b08[];
extern u8 data_0217fb08[];

/* declared `Int5 data_0216a870` in campaign_recap_update.c: declared in src/game/main/campaign_recap_update.c, with a type no header defines */
u32 data_0216a870[5] = {
#include "data/main/data_0216a870.inc.c"
};
/* declared `Int5 data_0216a884` in campaign_recap_update.c: declared in src/game/main/campaign_recap_update.c, with a type no header defines */
u32 data_0216a884[5] = {
#include "data/main/data_0216a884.inc.c"
};
TaskScriptCmd data_0216a898[3] = {
#include "data/main/data_0216a898.inc.c"
};
TaskScriptCmd data_0216a8b0[5] = {
#include "data/main/data_0216a8b0.inc.c"
};
/* declared `u8 data_0216a8d8[]` in campaign_recap_set_up_screens.c: relocation at data_0216a8d8+0xc is not on a 4-byte field */
u32 data_0216a8d8[20] = {
#include "data/main/data_0216a8d8.inc.c"
};
