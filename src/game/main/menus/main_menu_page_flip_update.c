#include "types.h"
#include "game/main_menu.h"

void Gfx3d_InitScene(void);
void G3_RotX(int sin, int cos);
void MainMenu_DrawPage(void);
void MainMenu_DrawPicture192x144(void);
void func_02028644(void);
void Gfx3d_SetOrthoCentered(void);
void Task_ResumeScript(void* task);

extern s16 data_0214d4c4[]; /* a sine and cosine pair per 16 angle units */

/* Task step of a page flip, 16 frames: the view turns about the vertical
 * axis (angle in the sine table's units, 16 per table entry). In the first
 * half MainMenu_DrawPicture192x144 draws, in the second half the new page. The counter
 * is a halfword in the task, read as signed to match. Resumes the script
 * at frame 16. */
void MainMenu_PageFlipUpdate(MainMenuTask* task) {
  u16 turn = 16 - task->unk_4c;
  int angle;
  int idx;

  task->unk_4c++;
  Gfx3d_InitScene();
  if (turn != 0) {
    angle = turn < 8 ? (u16)(-turn << 11) : (u16)(-(turn - 16) << 11);
    idx = angle >> 4;
    G3_RotX(data_0214d4c4[idx * 2], data_0214d4c4[idx * 2 + 1]);
    if (turn > 8) {
      MainMenu_DrawPicture192x144();
    } else if (turn < 8) {
      MainMenu_DrawPage();
    }
  } else {
    MainMenu_DrawPage();
  }
  func_02028644();
  if (task->unk_4c != 16) {
    return;
  }
  Gfx3d_SetOrthoCentered();
  task->unk_4c = 0;
  Task_ResumeScript(task);
}
