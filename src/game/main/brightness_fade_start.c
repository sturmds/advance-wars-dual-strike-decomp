#include "types.h"
#include "game/brightness_fade.h"
#include "game/screen_other_map.h"

/* Fade task scripts, one per screen and direction. */
extern const TaskScriptCmd data_0216a658[]; /* screen 0, out */
extern const TaskScriptCmd data_0216a668[]; /* screen 1, in */
extern const TaskScriptCmd data_0216a678[]; /* screen 0, in */
extern const TaskScriptCmd data_0216a688[]; /* screen 1, out */

Task* BrightnessFade_Start(void* parentOrList, int out, int screen, int white,
                           int frames) {
  BrightnessFadeTask* fade;
  Unk_027c0280* savedGfx;
  Unk_02080440* savedState;

  if (screen == 0) {
    if (out == 0) {
      fade = (BrightnessFadeTask*)Task_Create(data_0216a678, parentOrList);
    } else {
      fade = (BrightnessFadeTask*)Task_Create(data_0216a658, parentOrList);
    }
  } else {
    if (out == 0) {
      fade = (BrightnessFadeTask*)Task_Create(data_0216a668, parentOrList);
    } else {
      fade = (BrightnessFadeTask*)Task_Create(data_0216a688, parentOrList);
    }
  }
  fade->active = 1;
  fade->step = 0;
  fade->frame = 0;
  fade->frames = frames;
  if (white == 0) {
    if (out == 0) {
      fade->startLevel = -16;
      fade->endLevel = 0;
    } else {
      fade->startLevel = 0;
      fade->endLevel = -16;
    }
  } else {
    if (out == 0) {
      fade->startLevel = 16;
      fade->endLevel = 0;
    } else {
      fade->startLevel = 0;
      fade->endLevel = 16;
    }
  }
  fade->screen = screen;

  /* Set the starting level on the faded screen's display state. */
  if (screen == 0) {
    savedGfx = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_00);
    savedState = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_00);
    data_027c0280->masterBrightness = fade->startLevel;
    Screen_SetCurrent(savedState);
    Screen_SetCurrentGfx(savedGfx);
  } else {
    savedGfx = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_04);
    savedState = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_04);
    data_027c0280->masterBrightness = fade->startLevel;
    Screen_SetCurrent(savedState);
    Screen_SetCurrentGfx(savedGfx);
  }
  return (Task*)fade;
}
