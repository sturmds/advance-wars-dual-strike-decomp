#include "types.h"
#include "game/task.h"
#include "game/Unk_02290118.h"
#include "game/screen_other_map.h"

void CampaignMap_StartMode0(void);
void Map_InitBattle(void);
void Map_SetupPlayerCos(void);
void SetupMapRules(void);
void Survival_LoadPlayerCos(void);
void Survival_SetCurrentMap(void);
extern u32 data_0216710c[];
void func_02048238(void);
void func_020b983c(void);
void func_020fd85c(void);
void func_020e8970(void);
void func_02050664(void);

TaskScriptCmd data_021670d4[] = {
#include "data/main/data_021670d4.inc.c"
};
u32 data_0216710c[] = {
#include "data/main/data_0216710c.inc.c"
};
TaskScriptCmd gSurvivalResumeScript[] = {
#include "data/main/gSurvivalResumeScript.inc.c"
};

int Campaign_FindFreeProfile(void);
void Campaign_ResetProgress(void);
void ForceRank_ClearSavedSets(void);
void PlayRecords_TakeSnapshot(void);
void Screen_SetCurrentBoth(void* state);
void ForceRank_LoadSets(int mode);

/* Sets up a new hard-mode campaign: picks a profile slot, resets the progress,
 * puts both screens in game mode 0, clears the campaign state and starts the
 * data_021670d4 task. Campaign_StartNormal is the same with hardMode 0. */
void Campaign_StartHard(void) {
  int slot = Campaign_FindFreeProfile();
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;

  Campaign_ResetProgress();
  saved0280 = data_027c0280;
  saved027c = data_027c027c;
  Screen_SetCurrentBoth(data_027c0288.unk_00);
  data_027c027c->unk_48[data_027c0284]->gameMode = GAME_MODE_CAMPAIGN;
  Screen_SetCurrentBoth(data_027c0288.unk_04);
  data_027c027c->unk_48[data_027c0284]->gameMode = GAME_MODE_CAMPAIGN;
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
  data_02290118.unk_01 = slot;
  data_02290118.unk_00 = 0;
  data_02290118.unk_04 = 0;
  data_02290118.unk_06 = 0;
  data_02290118.unk_05 = 0;
  data_02290118.unk_08 = 0;
  data_02290118.unk_0a = 0;
  data_02290118.unk_0c = 0;
  data_02290118.unk_0e = 0;
  data_02290118.hardMode = 1;
  ForceRank_ClearSavedSets();
  ForceRank_LoadSets(0);
  PlayRecords_TakeSnapshot();
  Task_Create(data_021670d4, TASK_LIST(3));
}
