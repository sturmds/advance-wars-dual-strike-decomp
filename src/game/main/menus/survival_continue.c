#include "types.h"
#include "game/task.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/screen_other_map.h"

void Screen_SetCurrentBoth(void* state);
int BattleSave_GetRecordId(int gameMode, int campaignSlot);
u8 func_020193bc(int recordId);
int func_0201fe28(int recordId);
void func_02017cf8(int recordId);
void ForceRank_LoadSets(int mode);

extern const TaskScriptCmd gSurvivalResumeScript[];

/* Sets game mode Survival on both screens. When the survival battle save
 * exists and is usable, it starts loading it. Otherwise it loads the
 * Survival Force Rank sets and starts the run restore script. */
void Survival_Continue(void) {
  int recordId = BattleSave_GetRecordId(GAME_MODE_SURVIVAL, 0);
  Unk_027c0280* savedGfx = data_027c0280;
  Unk_02080440* savedState = data_027c027c;

  Screen_SetCurrentBoth(data_027c0288.unk_00);
  MAP_STATE->gameMode = GAME_MODE_SURVIVAL;
  Screen_SetCurrentBoth(data_027c0288.unk_04);
  MAP_STATE->gameMode = GAME_MODE_SURVIVAL;
  Screen_SetCurrent(savedState);
  Screen_SetCurrentGfx(savedGfx);
  if (func_020193bc(recordId) && !func_0201fe28(recordId)) {
    func_02017cf8(recordId);
    return;
  }
  ForceRank_LoadSets(GAME_MODE_SURVIVAL);
  Task_Create(gSurvivalResumeScript, TASK_LIST(3));
}
