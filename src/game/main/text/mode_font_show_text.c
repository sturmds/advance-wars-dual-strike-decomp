#include "types.h"

/* starts a task that shows text (A-Z, '*', '-') centred, in the font from
 * /ohashi/res_modefont, on the screen in data_027c0288.unk_04 */
void ModeFont_ShowTextOnScreen(const char* text, int tile, int paletteSlot,
                               int unk_56, u8 unk_5c, int unk_58);

/* tile and paletteSlot: where ModeFont_Load put the font. */
void ModeFont_ShowText(const char* text, int tile, int paletteSlot, u8 unk_5c,
                       int unk_58) {
  ModeFont_ShowTextOnScreen(text, tile, paletteSlot, 0, unk_5c, unk_58);
}
