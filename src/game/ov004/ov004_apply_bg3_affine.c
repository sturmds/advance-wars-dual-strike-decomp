#include "types.h"
#include "game/Unk_ov004_022bf158.h"
#include "game/screen_other_map.h"

/* Sets the first screen's BG3 affine matrix from the affine parameters in the state. */
void Ov004_ApplyBg3Affine(Unk_ov004_022bf158* state) {
  Unk_027c0280* saved0280 = data_027c0280;
  Unk_02080440* saved027c;

  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  BgAffine_FromParams(&state->bg3Params, &data_027c0280->bg3Affine);
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
}
