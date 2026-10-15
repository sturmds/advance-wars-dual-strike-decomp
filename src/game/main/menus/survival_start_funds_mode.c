#include "types.h"
#include "game/task.h"
#include "game/Unk_02080440.h"
#include "game/Unk_022a7b80.h"
#include "game/Unk_02088d04.h"
#include "game/screen_other_map.h"

void Screen_SetCurrentBoth(void* state);
void ForceRank_LoadSets(int mode);

extern Unk_022a7b80 data_022a7b80;
extern const TaskScriptCmd gSurvivalNewRunScript[];

/* Same as Survival_StartTimeMode for mode 1 (funds budget). */
void Survival_StartFundsMode(void) {
  Unk_027c0280* savedGfx = data_027c0280;
  Unk_02080440* savedState = data_027c027c;

  Screen_SetCurrentBoth(data_027c0288.unk_00);
  MAP_STATE->gameMode = GAME_MODE_SURVIVAL;
  Screen_SetCurrentBoth(data_027c0288.unk_04);
  MAP_STATE->gameMode = GAME_MODE_SURVIVAL;
  Screen_SetCurrent(savedState);
  Screen_SetCurrentGfx(savedGfx);
  data_022a7b80.mode = 1;
  ForceRank_LoadSets(GAME_MODE_SURVIVAL);
  Task_Create(gSurvivalNewRunScript, TASK_LIST(3));
}
