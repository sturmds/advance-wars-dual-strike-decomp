#include "types.h"
#include "game/screen_other_map.h"
#include "game/menu_state.h"

int MainLoop_GetFrameCount(void);
void Palette_Load(const void* src, u32 offset, u32 size);

/* On the sub screen, copies one of 16 colours stored at the start of
 * gMenuState to palette offset 0x218, stepping every 4 frames. */
void SubScreen_CyclePaletteColor(void) {
  Unk_027c0280* saved0280 = data_027c0280;
  Unk_02080440* saved027c;

  Screen_SetCurrentGfx(data_027c0288.unk_04);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  Palette_Load(&((u16*)&gMenuState)[(MainLoop_GetFrameCount() >> 2) & 15],
               0x218, 2);
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
}
