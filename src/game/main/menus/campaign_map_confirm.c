#include "types.h"
#include "game/campaign_map.h"
#include "game/menu_button.h"

CampaignMapTask* CampaignMap_FindTask(void);
void func_020b9890(void);
void Task_JumpToLabel(void* task, int label);
int Sound_PlaySe(int id);

/* A or Start pressed: closes the buttons, restarts the task's step counter
 * and sends it to label 128. The name is a guess from the key. */
void CampaignMap_Confirm(int unused) {
  CampaignMapTask* task = CampaignMap_FindTask();

  MenuButtons_CloseAll();
  MenuButton_Remove(252);
  Sound_PlaySe(0x10006);
  func_020b9890();
  task->step = 0;
  Task_JumpToLabel(task, 128);
}
