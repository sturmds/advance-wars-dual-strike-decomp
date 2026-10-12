#include "types.h"
#include "game/Unk_02080440.h"
#include "game/win_lose.h"

void Gfx_RequestLayerCopy(int layer);
void Task_ResumeScript(WinLoseTask* task);
void Gfx_ClearLayerMap(int layer); /* likely clears the layer's tile map */
void func_0201e6e8(int x, int y, int width,
                   int height); /* window frame on layer 1 */
void WinLoseScreen_DrawPicturesTwoSides(WinLoseTask* task);

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define GAME (data_027c027c->unk_48[data_027c0284])

/* Task update of the win/lose screen: keeps the two-side CO pictures
 * drawn and shrinks the window frame on layer 1 around column 15 by one
 * column per side each frame while task->frame counts down (from 12,
 * set by WinLoseScreen_ShowSurvivalStats). Survival uses a taller frame
 * at row 14. Then resumes the task's script; at once in game mode 2. */
void WinLoseScreen_ClosePanel(WinLoseTask* task) {
  WinLoseScreen_DrawPicturesTwoSides(task);
  if (GAME->gameMode == 2) { /* likely Versus */
    task->frame = 0;
    Task_ResumeScript(task);
    return;
  }
  Gfx_ClearLayerMap(1);
  if (task->frame >= 0) {
    if (GAME->gameMode == GAME_MODE_SURVIVAL) {
      func_0201e6e8((s16)(14 - task->frame), 14, (s16)(task->frame * 2 + 2), 8);
    } else {
      func_0201e6e8((s16)(14 - task->frame), 15, (s16)(task->frame * 2 + 2), 6);
    }
  }
  Gfx_RequestLayerCopy(1);
  if (task->frame != -1) {
    task->frame--;
    return;
  }
  task->frame = 0;
  Task_ResumeScript(task);
}
