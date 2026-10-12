#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"

void Screen_SetCurrent(void* state);
void Screen_SetCurrentGfx(void* state);
void ModeFont_StartTextTask(const char* text, int tile, int paletteSlot,
                            int unk_56, u8 unk_5c, int unk_58);

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern Unk_027c0288 data_027c0288;

/* Starts the text task (ModeFont_StartTextTask) with the screen state in
 * data_027c0288.unk_04 current, then restores the previous state.
 * ModeFont_ShowText calls it with unk_56 = 0. */
void ModeFont_ShowTextOnScreen(const char* text, int tile, int paletteSlot,
                               int unk_56, u8 unk_5c, int unk_58) {
  Unk_027c0280* saved0280 = data_027c0280;
  Unk_02080440* saved027c;

  Screen_SetCurrentGfx(data_027c0288.unk_04);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  ModeFont_StartTextTask(text, tile, paletteSlot, unk_56, unk_5c, unk_58);
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
}
