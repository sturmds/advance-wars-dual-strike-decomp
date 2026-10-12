#include "types.h"
#include "game/screen_other_map.h"
#include "game/tag_power_scroll_task.h"

/* Lines on one screen. The screen in data_027c0288.unk_00 scrolls this much
 * further than the other, so the two show one continuous background. */
#define SCREEN_HEIGHT 192

/* Sets one layer's vertical scroll on the given screen, then switches back.
 * A macro, so that y is read after the switch as in the original. */
#define SET_SCROLL_Y(screen, layerBg, y)                    \
  do {                                                      \
    savedGfx = data_027c0280;                               \
    Screen_SetCurrentGfx(screen);                           \
    savedState = data_027c027c;                             \
    Screen_SetCurrent(screen);                              \
    data_027c0280->bgScrollY[data_027c0280->layerBg] = (y); \
    Screen_SetCurrent(savedState);                          \
    Screen_SetCurrentGfx(savedGfx);                         \
  } while (0)

/* Update of the task the Tag Power cut-in script runs beside its steps:
 * scrolls BG layer 1 by 24 and layer 2 by 12 lines a frame on both screens. */
void TagPowerScreen_ScrollBackground(TagPowerScrollTask* task) {
  Unk_027c0280* savedGfx;
  Unk_02080440* savedState;

  task->layer1Y += 24;
  task->layer2Y += 12;
  SET_SCROLL_Y(data_027c0288.unk_00, layer1Bg, task->layer1Y + SCREEN_HEIGHT);
  SET_SCROLL_Y(data_027c0288.unk_04, layer1Bg, task->layer1Y);
  SET_SCROLL_Y(data_027c0288.unk_00, layer2Bg, task->layer2Y + SCREEN_HEIGHT);
  SET_SCROLL_Y(data_027c0288.unk_04, layer2Bg, task->layer2Y);
}
