#include "types.h"
#include "game/co_portrait_task.h"
#include "game/Unk_02160ac0.h"

void Co_LoadGraphicsAndPalette(int coId, int tile, int paletteSlot);
void Co_UnpackPicture(int coId, int tile);
void Palette_LoadObjRow(int row);
void DrawSelectedCoPowerText(void);

extern u8 gCombatModeCos[];

/* Init step of the CO portrait task of the Combat mode menu: the portrait
 * starts in place, showing the CO of the slot selected in data_02160ac0->co. */
void CombatModeCoPortrait_Init(CoPortraitTask* task) {
  task->slideFrom = 0;
  task->offset = 0;
  task->selectedSlot = data_02160ac0->co;
  task->shownSlot = task->selectedSlot;
  task->frame = CO_PORTRAIT_SLIDE_FRAMES;
  task->waitFrames = 0;
  task->slidingIn = 1;
  Co_LoadGraphicsAndPalette(gCombatModeCos[task->selectedSlot], 0, 0);
  Co_UnpackPicture(gCombatModeCos[task->selectedSlot], 0x1b0);
  Palette_LoadObjRow(1);
  DrawSelectedCoPowerText();
}
