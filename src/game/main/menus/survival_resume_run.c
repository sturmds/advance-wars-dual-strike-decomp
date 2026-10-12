#include "types.h"
#include "game/task.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"

void Screen_SetCurrent(void* state);
void Screen_SetCurrentGfx(void* state);
void Screen_SetCurrentBoth(void* state);

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern int data_027c0284;
extern Unk_027c0288 data_027c0288;
extern const TaskScriptCmd gSurvivalResumeScript[];

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Sets game mode Survival on both screens, then starts the script that
 * restores the run's map and player COs (Survival_SetCurrentMap,
 * Survival_LoadPlayerCos). */
void Survival_ResumeRun(void) {
  Unk_027c0280* savedGfx = data_027c0280;
  Unk_02080440* savedState = data_027c027c;

  Screen_SetCurrentBoth(data_027c0288.unk_00);
  STATE->gameMode = GAME_MODE_SURVIVAL;
  Screen_SetCurrentBoth(data_027c0288.unk_04);
  STATE->gameMode = GAME_MODE_SURVIVAL;
  Screen_SetCurrent(savedState);
  Screen_SetCurrentGfx(savedGfx);
  Task_Create(gSurvivalResumeScript, TASK_LIST(3));
}
