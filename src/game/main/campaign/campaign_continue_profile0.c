#include "types.h"
#include "game/task.h"
#include "game/Unk_02290118.h"
#include "game/screen_other_map.h"

extern const TaskScriptCmd data_021670a4[];

int BattleSave_GetRecordId(int mode, int campaignSlot);
u8 func_020193bc(int recordId);
int func_0201fe28(int recordId);
void func_02017cf8(int recordId);
void Save_LoadProfile(int profile);
void PlayRecords_TakeSnapshot(void);
void func_02103744(void);
void Screen_SetCurrentBoth(void* state);
void ForceRank_LoadSets(int mode);

/* Campaign menu entry for profile 0 (the menu table next to Campaign_StartHard
 * holds one entry per profile). Puts both screens in campaign mode and loads
 * the profile. With unk_04 0xff it starts the func_02103744 task. If the
 * profile has a usable battle save, func_02017cf8 resumes it; otherwise the
 * Force Rank sets are loaded and the data_021670a4 task starts. */
void Campaign_ContinueProfile0(void) {
  int recordId = BattleSave_GetRecordId(0, 0);
  Unk_027c0280* saved0280 = data_027c0280;
  Unk_02080440* saved027c = data_027c027c;

  Screen_SetCurrentBoth(data_027c0288.unk_00);
  data_027c027c->unk_48[data_027c0284]->gameMode = GAME_MODE_CAMPAIGN;
  Screen_SetCurrentBoth(data_027c0288.unk_04);
  data_027c027c->unk_48[data_027c0284]->gameMode = GAME_MODE_CAMPAIGN;
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
  PlayRecords_TakeSnapshot();
  Save_LoadProfile(0);
  if (data_02290118.unk_04 == 0xff) {
    func_02103744();
    return;
  }
  if (func_020193bc(recordId) && !func_0201fe28(recordId)) {
    func_02017cf8(recordId);
    return;
  }
  ForceRank_LoadSets(0);
  Task_Create(data_021670a4, TASK_LIST(3));
}
