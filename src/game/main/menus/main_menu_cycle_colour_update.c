#include "types.h"
#include "game/main_menu.h"

void func_0210c8e8(void);
void GX_EndLoadTexPltt(void);

#define VRAM_LCDC_F ((u16 *)0x06890000)
#define WORK ((MainMenuWork *)data_0229c71c)

/* Every fourth frame, sets colour 12 of the 9 palettes in bank F to the
 * next of the 16 colours in the work block's palette. */
void MainMenu_CycleColourUpdate(MainMenuTask *task)
{
    int i;

    if ((task->unk_4c & 3) == 0) {
        func_0210c8e8();
        for (i = 0; i < 9; i++) {
            (&VRAM_LCDC_F[i * 16])[12] = ((u16 *)WORK->unk_1c)[task->unk_52 & 15];
        }
        GX_EndLoadTexPltt();
        task->unk_52++;
    }
    task->unk_4c++;
}
