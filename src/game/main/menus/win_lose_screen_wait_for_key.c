#include "types.h"
#include "game/keypad.h"
#include "game/Unk_02080440.h"
#include "game/win_lose.h"

void ResultStats_ClosePage(void);
void WinLoseScreen_DrawPicturesTwoSides(WinLoseTask* task);
void WinLoseScreen_DrawPointsBox(s32 added, s32 total, int popFrame);

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define GAME (data_027c027c->unk_48[data_027c0284])

/* Task update of the win/lose screen once the points are shown: keeps the
 * pictures and the points drawn (no points in Versus) until A, B or Start
 * closes the page. */
void WinLoseScreen_WaitForKey(WinLoseTask* task) {
  WinLoseScreen_DrawPicturesTwoSides(task);
  if (GAME->gameMode != GAME_MODE_VERSUS) {
    WinLoseScreen_DrawPointsBox(task->pointsAdded, task->pointsTotal, -1);
  }
  if (gKeypad->pressed & (PAD_BUTTON_A | PAD_BUTTON_B | PAD_BUTTON_START)) {
    ResultStats_ClosePage();
  }
}
