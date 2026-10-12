#include "types.h"
#include "game/palette_fade.h"
#include "game/screen_other_map.h"

extern const TaskScriptCmd data_0216a698[];

void* Mem_Alloc(u32 size);
void Mem_Copy32(const void* src, void* dst, u32 size);
void Palette_Flush(void);

Task* PaletteFade_Start(void* parentOrList, int fromCurrent, const void* colors,
                        int screen, int frames, int firstPalette,
                        int paletteCount, int startLevel, int endLevel) {
  PaletteFadeTask* fade;
  Unk_027c0280* savedGfx;
  Unk_02080440* savedState;

  fade = (PaletteFadeTask*)Task_Create(data_0216a698, parentOrList);
  fade->active = 1;
  fade->step = 0;
  fade->frame = 0;
  fade->frames = frames;
  fade->firstPalette = firstPalette;
  fade->paletteCount = paletteCount;
  fade->startLevel = startLevel;
  fade->endLevel = endLevel;
  fade->screen = screen;
  if (screen == 0) {
    savedGfx = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_00);
    savedState = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_00);
    fade->palettes = data_027c0280->paletteBuffer + fade->firstPalette * 32;
    Palette_Flush();
    Screen_SetCurrent(savedState);
    Screen_SetCurrentGfx(savedGfx);
  } else {
    savedGfx = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_04);
    savedState = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_04);
    fade->palettes = data_027c0280->paletteBuffer + fade->firstPalette * 32;
    Palette_Flush();
    Screen_SetCurrent(savedState);
    Screen_SetCurrentGfx(savedGfx);
  }
  fade->colors = Mem_Alloc(fade->paletteCount * 32);
  if (fromCurrent == 0) {
    /* Fade in to the given colors: start from black. */
    Mem_Copy32(colors, fade->colors, fade->paletteCount * 32);
    Palette_Darken(fade->palettes, 0, fade->paletteCount, 16);
  } else {
    Mem_Copy32(fade->palettes, fade->colors, fade->paletteCount * 32);
  }
  return (Task*)fade;
}
