#include "types.h"
#include "game/task.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/ov003_unit.h"

void Screen_SetCurrent(void* state);
void Screen_SetCurrentGfx(void* state);
extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern Unk_027c0288 data_027c0288;
extern const TaskScriptCmd data_ov003_02364b74[];

/* Sets Ov003State.nextPhase to 6 and starts the
 * task of data_ov003_02364b74 on the first screen. */
void func_ov003_0236322c(void) {
  Unk_027c0280* savedGfx = data_027c0280;
  Unk_02080440* saved027c;

  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  data_ov003_02364928->phases = (data_ov003_02364928->phases & ~0xf0) | 0x60;
  Task_Create(data_ov003_02364b74, TASK_LIST(4));
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(savedGfx);
}
