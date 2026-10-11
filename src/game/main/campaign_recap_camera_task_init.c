#include "types.h"
#include "game/campaign_recap.h"

extern CampaignRecap *data_022a7ea4;

void CampaignRecap_SetCamera(CampaignRecap *recap, int x, int y, int z);

/* Puts the camera at its start position and starts the task's steps. */
void CampaignRecapCameraTask_Init(CampaignRecapCameraTask *task)
{
    CampaignRecap *recap = data_022a7ea4;

    CampaignRecap_SetCamera(recap, recap->cameraStartX, recap->cameraStartY, recap->cameraStartZ);
    task->step = 0;
}
