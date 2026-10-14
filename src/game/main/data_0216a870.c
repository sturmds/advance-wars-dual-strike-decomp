#include "types.h"
#include "game/Unk_02088d04.h"
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
u32 data_0216a870[] = {
#include "data/main/data_0216a870.inc.c"
};
/* declared `Int5 data_0216a884` in campaign_recap_update.c: declared in src/game/main/campaign_recap_update.c, with a type no header defines */
u32 data_0216a884[] = {
#include "data/main/data_0216a884.inc.c"
};
TaskScriptCmd data_0216a898[] = {
#include "data/main/data_0216a898.inc.c"
};
TaskScriptCmd data_0216a8b0[] = {
#include "data/main/data_0216a8b0.inc.c"
};
GfxLayer data_0216a8d8[] = {
#include "data/main/data_0216a8d8.inc.c"
};
