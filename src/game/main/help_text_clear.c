#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"

void Gfx_RequestLayerCopy(int a);
void Screen_SetCurrent(void* state);
void Screen_SetCurrentGfx(void* state);
void Bg_ClearRect(void* target, int x, int y, int w, int h);

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern Unk_027c0288 data_027c0288;

/* Clears the help text band (tile rows 22-23 of layer 0) on the lower screen. */
void HelpText_Clear(void) {
  Unk_027c0280* saved0280 = data_027c0280;
  Unk_02080440* saved027c;

  Screen_SetCurrentGfx(data_027c0288.unk_04);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  Bg_ClearRect(data_027c0280->layer0Map, 0, 22, 32, 2);
  Gfx_RequestLayerCopy(0);
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
}
