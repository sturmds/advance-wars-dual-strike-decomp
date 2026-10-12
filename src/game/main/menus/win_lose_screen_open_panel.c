#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/win_lose.h"

void Screen_SetCurrent(void* state);
void Screen_SetCurrentGfx(void* state);
void Task_ResumeScript(WinLoseTask* task);
void func_0201e6e8(int x, int y, int width,
                   int height); /* window frame on layer 1 */
void WinLoseScreen_DrawPicturesTwoSides(WinLoseTask* task);

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern int data_027c0284;
extern Unk_027c0288 data_027c0288;

#define GAME (data_027c027c->unk_48[data_027c0284])

/* Task update of the win/lose screen, the reverse of
 * WinLoseScreen_ClosePanel: on the main screen grows the window frame on
 * layer 1 around column 15 by one column per side each frame, counting
 * task->frame up to 12 in Survival (taller frame at row 14) and to 10
 * otherwise, then resumes the task's script; at once in game mode 2. */
void WinLoseScreen_OpenPanel(WinLoseTask* task) {
  WinLoseScreen_DrawPicturesTwoSides(task);
  if (GAME->gameMode == 2) { /* likely Versus */
    Task_ResumeScript(task);
    return;
  }
  {
    Unk_027c0280* saved0280 = data_027c0280;
    Unk_02080440* saved027c;

    Screen_SetCurrentGfx(data_027c0288.unk_00);
    saved027c = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_00);
    if (GAME->gameMode == GAME_MODE_SURVIVAL) {
      func_0201e6e8((s16)(14 - task->frame), 14, (s16)(task->frame * 2 + 2), 8);
    } else {
      func_0201e6e8((s16)(14 - task->frame), 15, (s16)(task->frame * 2 + 2), 6);
    }
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);
  }
  if (task->frame == 12 ||
      (task->frame == 10 && GAME->gameMode != GAME_MODE_SURVIVAL)) {
    task->frame = 0;
    Task_ResumeScript(task);
    return;
  }
  task->frame++;
}
