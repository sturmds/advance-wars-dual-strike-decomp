#include "types.h"
#include "game/campaign_recap.h"
#include "game/task.h"
#include "game/math.h"

extern CampaignRecap* data_022a7ea4;

void Task_ResumeScript(Task* task);
void CampaignRecap_SetCamera(CampaignRecap* recap, int x, int y, int z);

void CampaignRecap_UpdateCamera(CampaignRecapCameraTask* task) {
  CampaignRecap* recap = data_022a7ea4;
  int x;
  int y;
  int z;

  switch (task->step) {
    case 0:
      task->frame = 0;
      task->frames = 330;
      task->step++;
    case 1:
      x = Math_Ease(0, recap->cameraStartX, recap->cameraEndX, task->frame,
                    task->frames);
      y = Math_Ease(0, recap->cameraStartY, recap->cameraEndY, task->frame,
                    task->frames);
      z = Math_Ease(0, recap->cameraStartZ, recap->cameraEndZ, task->frame,
                    task->frames);
      CampaignRecap_SetCamera(recap, x, y, z);
      if (task->frame <= task->frames) {
        task->frame++;
      }
      if (task->frame > task->frames) {
        task->step++;
      }
      break;
    case 2:
      Task_ResumeScript((Task*)task);
      break;
  }
}
