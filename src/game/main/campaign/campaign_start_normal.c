#include "types.h"
#include "game/task.h"
#include "game/Unk_02290118.h"
#include "game/screen_other_map.h"

extern const TaskScriptCmd data_021670d4[];

int Campaign_FindFreeProfile(void);
void Campaign_ResetProgress(void);
void ForceRank_ClearSavedSets(void);
void PlayRecords_TakeSnapshot(void);
void Screen_SetCurrentBoth(void* state);
void ForceRank_LoadSets(int mode);

/* Same as Campaign_StartHard with hardMode 0. */
void Campaign_StartNormal(void) {
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
  data_02290118.hardMode = 0;
  data_02290118.unk_04 = 0;
  data_02290118.unk_06 = 0;
  data_02290118.unk_05 = 0;
  data_02290118.unk_08 = 0;
  data_02290118.unk_0a = 0;
  data_02290118.unk_0c = 0;
  data_02290118.unk_0e = 0;
  ForceRank_ClearSavedSets();
  ForceRank_LoadSets(0);
  PlayRecords_TakeSnapshot();
  Task_Create(data_021670d4, TASK_LIST(3));
}
