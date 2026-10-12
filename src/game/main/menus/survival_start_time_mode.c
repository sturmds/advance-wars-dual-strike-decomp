#include "types.h"
#include "game/task.h"
#include "game/Unk_02080440.h"
#include "game/Unk_022a7b80.h"
#include "game/Unk_02088d04.h"

void Screen_SetCurrent(void* state);
void Screen_SetCurrentGfx(void* state);
void Screen_SetCurrentBoth(void* state);
void ForceRank_LoadSets(int mode);

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern int data_027c0284;
extern Unk_027c0288 data_027c0288;
extern Unk_022a7b80 data_022a7b80;
extern const TaskScriptCmd gSurvivalNewRunScript[];

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Starts a new Survival run in mode 0 (time budget): sets the game mode,
 * loads the Force Rank sets and starts the new run script. */
void Survival_StartTimeMode(void) {
  Unk_027c0280* savedGfx = data_027c0280;
  Unk_02080440* savedState = data_027c027c;

  Screen_SetCurrentBoth(data_027c0288.unk_00);
  STATE->gameMode = GAME_MODE_SURVIVAL;
  Screen_SetCurrentBoth(data_027c0288.unk_04);
  STATE->gameMode = GAME_MODE_SURVIVAL;
  Screen_SetCurrent(savedState);
  Screen_SetCurrentGfx(savedGfx);
  data_022a7b80.mode = 0;
  ForceRank_LoadSets(GAME_MODE_SURVIVAL);
  Task_Create(gSurvivalNewRunScript, TASK_LIST(3));
}
