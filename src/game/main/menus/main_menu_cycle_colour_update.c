#include "types.h"
#include "game/main_menu.h"

void GX_BeginLoadTexPltt(void);
void GX_EndLoadTexPltt(void);

#define VRAM_LCDC_F ((u16*)0x06890000)
#define WORK ((MainMenuWork*)data_0229c71c)

/* Every fourth frame, sets colour 12 of the 9 palettes in bank F to the
 * next of the 16 colours in the work block's palette. */
void MainMenu_CycleColourUpdate(MainMenuTask* task) {
  int i;

  if ((task->unk_4c & 3) == 0) {
    GX_BeginLoadTexPltt();
    for (i = 0; i < (int)ARRAY_COUNT(WORK->unk_08); i++) {
      (&VRAM_LCDC_F[i * 16])[12] = ((u16*)WORK->unk_1c)[task->unk_52 & 15];
    }
    GX_EndLoadTexPltt();
    task->unk_52++;
  }
  task->unk_4c++;
}
