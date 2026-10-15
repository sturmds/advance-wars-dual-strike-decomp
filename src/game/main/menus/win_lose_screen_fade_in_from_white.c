#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/win_lose.h"
#include "game/screen_other_map.h"

void Screen_SetCurrentBoth(void* state);
void Task_ResumeScript(WinLoseTask* task);

/* Task update of the win/lose screen: over 32 frames lowers the master
 * brightness of both screens from 16 (white) to 0, then resumes the script. */
void WinLoseScreen_FadeInFromWhite(WinLoseTask* task) {
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;

  task->frame++;
  saved0280 = data_027c0280;
  saved027c = data_027c027c;
  Screen_SetCurrentBoth(data_027c0288.unk_00);
  data_027c0280->masterBrightness = 16 - (task->frame >> 1);
  Screen_SetCurrentBoth(data_027c0288.unk_04);
  data_027c0280->masterBrightness = 16 - (task->frame >> 1);
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
  if (task->frame == 32) {
    task->frame = 0;
    Task_ResumeScript(task);
  }
}
