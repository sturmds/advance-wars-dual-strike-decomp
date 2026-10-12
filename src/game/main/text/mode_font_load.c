#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"

void Screen_SetCurrent(Unk_02080440* state);
void Screen_SetCurrentGfx(Unk_027c0280* state);
/* loads the /ohashi/res_modefont tiles to OBJ tiles tile and tile + 0xe0,
 * and its two palettes to OBJ palettes paletteSlot and paletteSlot + 1 */
void ModeFont_LoadHere(int tile, int paletteSlot);

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern Unk_027c0288 data_027c0288;

/* Loads the mode font on the screen in data_027c0288.unk_04, then
 * restores the current screen. */
void ModeFont_Load(int tile, int paletteSlot) {
  Unk_027c0280* savedGfx = data_027c0280;
  Unk_02080440* savedState;

  Screen_SetCurrentGfx(data_027c0288.unk_04);
  savedState = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  ModeFont_LoadHere(tile, paletteSlot);
  Screen_SetCurrent(savedState);
  Screen_SetCurrentGfx(savedGfx);
}
