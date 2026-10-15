#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/script_slot.h"
#include "game/task.h"
#include "game/screen_other_map.h"

void Task_DestroyByScript(const TaskScriptCmd* script);

extern TaskScriptCmd data_021590ec[];

/* Ends the help band: destroys its task (HelpBand_Start) and resets the lower
 * screen's layer 0 scroll, which the task moves. Also clears DISPCNT bit 15
 * of the lower screen. */
void HelpBand_Stop(void) {
  Unk_027c0280* saved0280 = data_027c0280;
  Unk_02080440* saved027c;

  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  Task_DestroyByScript(data_021590ec);
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);

  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  GFX_DISPCNT(data_027c0280)->objWin = 0;
  data_027c0280->bgScrollX[data_027c0280->layer0Bg] = 0;
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
}
