#include "types.h"
#include "game/co_portrait_task.h"

int Math_Ease(int curve, int from, int to, int elapsed, int duration);
void Sprite_QueueDefault(int x, int y, int attr, int list);
void Sprite_QueueAt(int list, int x, int y, void* data, int attr, int tile);
void func_0200270c(int x, int y, int attr, int list, int hFlip);
void Co_LoadGraphicsAndPalette(int coId, int tile, int paletteSlot);

extern u8 gCombatModeCos[];
extern u8 data_027c0040;

#define HIDDEN_WAIT_FRAMES 4

/* Per-frame update of the Combat mode CO portrait task. Moves the portrait
 * along its slide (out to the left, or back in), draws it with two other
 * sprites around it, and once a slide out has ended and the portrait has
 * stayed hidden for HIDDEN_WAIT_FRAMES more frames, loads the selected CO's
 * graphics and starts the slide back in. */
void CombatModeCoPortrait_Update(CoPortraitTask* task) {
  if (task->slidingIn == 0) {
    task->offset =
        task->slideFrom + Math_Ease(1, 0, CO_PORTRAIT_HIDDEN, task->frame,
                                    CO_PORTRAIT_SLIDE_FRAMES);
    if (task->offset > CO_PORTRAIT_HIDDEN) {
      task->offset = CO_PORTRAIT_HIDDEN;
    }
  } else {
    task->offset =
        Math_Ease(4, task->slideFrom, 0, task->frame, CO_PORTRAIT_SLIDE_FRAMES);
  }
  Sprite_QueueDefault(168, 88, 0x11b0, 2);
  func_0200270c((72 - task->offset) & 0x1ff, 160, 0x800, 3, 1);
  Sprite_QueueAt(2, 136, 104, &data_027c0040, 0x21bc, 0);
  if (task->frame == CO_PORTRAIT_SLIDE_FRAMES) {
    if (task->slidingIn != 0) {
      return;
    }
    if (task->waitFrames < HIDDEN_WAIT_FRAMES) {
      task->waitFrames++;
      return;
    }
    task->shownSlot = task->selectedSlot;
    Co_LoadGraphicsAndPalette(gCombatModeCos[task->shownSlot], 0, 0);
    task->slideFrom = task->offset;
    task->slidingIn = 1;
    task->frame = 0;
    task->waitFrames = 0;
  } else {
    task->frame++;
  }
}
