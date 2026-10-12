#include "types.h"
#include "game/palette_fade.h"
#include "game/screen_other_map.h"

int Math_Ease(int curve, int from, int to, int elapsed, int duration);
void Mem_Copy32(const void* src, void* dst, u32 size);
void Mem_Free(void* block);
void Palette_Flush(void);

void PaletteFade_Update(PaletteFadeTask* fade, int startLevel, int endLevel) {
  u8* palettes = fade->palettes;
  int screen = fade->screen;
  int level;
  Unk_027c0280* savedGfx;
  Unk_02080440* savedState;

  switch (fade->step) {
    case 0:
      fade->step++;
    case 1:
      level = Math_Ease(0, startLevel, endLevel, fade->frame, fade->frames);
      Mem_Copy32(fade->colors, palettes, fade->paletteCount * 32);
      Palette_Darken(palettes, 0, fade->paletteCount, level);
      if (screen == 0) {
        savedGfx = data_027c0280;
        Screen_SetCurrentGfx(data_027c0288.unk_00);
        savedState = data_027c027c;
        Screen_SetCurrent(data_027c0288.unk_00);
        Palette_Flush();
        Screen_SetCurrent(savedState);
        Screen_SetCurrentGfx(savedGfx);
      } else {
        savedGfx = data_027c0280;
        Screen_SetCurrentGfx(data_027c0288.unk_04);
        savedState = data_027c027c;
        Screen_SetCurrent(data_027c0288.unk_04);
        Palette_Flush();
        Screen_SetCurrent(savedState);
        Screen_SetCurrentGfx(savedGfx);
      }
      if (fade->frame < fade->frames) {
        fade->frame++;
      } else {
        fade->step++;
      }
      break;
    case 2:
      Mem_Free(fade->colors);
      fade->colors = NULL;
      fade->step++;
      break;
    case 3:
      fade->active = 0;
      break;
  }
}
