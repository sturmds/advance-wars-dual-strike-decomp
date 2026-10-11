#include "types.h"
#include "game/campaign_map.h"
#include "game/menu_button.h"

CampaignMapTask *func_0204e5c0(void);
void Task_JumpToLabel(void *task, int label);
int Sound_PlaySe(int id);

/* B pressed: closes the buttons and sends the running task to label 255. The
 * meaning of the label is a guess from the key it follows. */
void CampaignMap_Cancel(int unused)
{
    CampaignMapTask *task = func_0204e5c0();

    MenuButtons_CloseAll();
    MenuButton_Remove(252);
    Sound_PlaySe(0x10009);
    Task_JumpToLabel(task, 255);
}
