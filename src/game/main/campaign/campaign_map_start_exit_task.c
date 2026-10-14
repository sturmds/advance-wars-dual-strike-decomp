#include "types.h"
#include "game/task.h"
#include "game/screen_other_map.h"

void CampaignMap_StartAfterBattle(void);
extern TaskScriptCmd data_0216710c[];
void func_02001588(void);
void func_020b983c(void);
void DesignMap_RefreshAllRows(void);
void func_0210372c(void);

char gPowerHighlightDir[] = {
#include "data/main/gPowerHighlightDir.inc.c"
};
TaskScriptCmd data_0216704c[] = {
#include "data/main/data_0216704c.inc.c"
};
static TaskScriptCmd data_02167074[] = {
#include "data/main/data_02167074.inc.c"
};

void Screen_SetCurrentBoth(void* state);

/* Sets the game mode of both screens' states to campaign, then starts the
 * task that data_02167074 scripts. Campaign_FinishMap runs it when the map
 * was not the last mission. */
void CampaignMap_StartExitTask(void) {
  Unk_027c0280* saved0280 = data_027c0280;
  Unk_02080440* saved027c = data_027c027c;

  Screen_SetCurrentBoth(data_027c0288.unk_00);
  data_027c027c->unk_48[data_027c0284]->gameMode = GAME_MODE_CAMPAIGN;
  Screen_SetCurrentBoth(data_027c0288.unk_04);
  data_027c027c->unk_48[data_027c0284]->gameMode = GAME_MODE_CAMPAIGN;
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
  Task_Create(data_02167074, TASK_LIST(3));
}
