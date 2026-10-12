#include "types.h"
#include "game/brightness_fade.h"
#include "game/screen_other_map.h"

int Math_Ease(int curve, int from, int to, int elapsed, int duration);

void BrightnessFade_Update(BrightnessFadeTask* fade) {
  int start = fade->startLevel;
  int end = fade->endLevel;
  int level;
  Unk_027c0280* savedGfx;
  Unk_02080440* savedState;

  switch (fade->step) {
    case 0:
      level = Math_Ease(0, start, end, fade->frame, fade->frames);
      if (fade->screen == 0) {
        savedGfx = data_027c0280;
        Screen_SetCurrentGfx(data_027c0288.unk_00);
        savedState = data_027c027c;
        Screen_SetCurrent(data_027c0288.unk_00);
        data_027c0280->masterBrightness = level;
        Screen_SetCurrent(savedState);
        Screen_SetCurrentGfx(savedGfx);
      } else {
        savedGfx = data_027c0280;
        Screen_SetCurrentGfx(data_027c0288.unk_04);
        savedState = data_027c027c;
        Screen_SetCurrent(data_027c0288.unk_04);
        data_027c0280->masterBrightness = level;
        Screen_SetCurrent(savedState);
        Screen_SetCurrentGfx(savedGfx);
      }
      if (fade->frame < fade->frames) {
        fade->frame++;
      } else {
        fade->step++;
      }
      break;
    case 1:
      fade->step++;
      break;
    case 2:
      fade->active = 0;
      break;
  }
}
