#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/task.h"

void Screen_SetCurrent(void* state);
void Screen_SetCurrentGfx(void* state);

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern Unk_027c0288 data_027c0288;
extern const TaskScriptCmd data_021613d8[]; /* script of the CO portrait task */

/* Starts the CO portrait task of the Combat mode menu on the second screen. */
void CombatModeCoPortrait_Start(void) {
  Unk_027c0280* savedGfx;
  Unk_02080440* savedScreen;

  savedGfx = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  savedScreen = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  Task_Create(data_021613d8, TASK_LIST(3));
  Screen_SetCurrent(savedScreen);
  Screen_SetCurrentGfx(savedGfx);
}
