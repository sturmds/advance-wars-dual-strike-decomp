#include "types.h"
#include "game/main_menu.h"

#define GXFIFO_MTX_TRANS ((volatile u32 *)0x04000470)

const void *Text_Get(u32 textId);
void ModeFont_ShowText(const void *text, int tile, int paletteSlot, int a, int b);
void Gfx3d_InitScene(void);
int Math_Ease(int curve, int from, int to, int elapsed, int duration);
void MainMenu_DrawPicture192x144(void);
void func_02028644(void);
void Sound_PlaySe(int id);
void Task_ResumeScript(void *task);

/* Task step of the menu's opening, 56 frames: shows the title at frame 0,
 * and for the first 40 frames the page comes down from 256 units above (eased
 * Y translation). Plays sound 76 and resumes the script at frame 56. */
void MainMenu_OpenUpdate(MainMenuTask *task)
{
    int frame;
    int offsetY;

    if (task->unk_4c == 0) {
        ModeFont_ShowText(Text_Get(0x7d00000a), 78, 1, 0, 1);
    }
    task->unk_4c++;
    Gfx3d_InitScene();
    frame = task->unk_4c;
    if (frame < 40) {
        offsetY = Math_Ease(4, 256, 0, frame, 40);
        *GXFIFO_MTX_TRANS = 0;
        *GXFIFO_MTX_TRANS = offsetY;
        *GXFIFO_MTX_TRANS = 0;
    }
    MainMenu_DrawPicture192x144();
    func_02028644();
    if (task->unk_4c != 56) {
        return;
    }
    Sound_PlaySe(76);
    task->unk_4c = 0;
    Task_ResumeScript(task);
}
