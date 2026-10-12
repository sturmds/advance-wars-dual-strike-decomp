#include "types.h"
#include "game/menu_button.h"
#include "game/Unk_ov005_0236bba8.h"

extern Unk_ov005_0236bba8* data_ov005_0236bba8;

/* Palette buttons have ids 0xe0 + variant (see Ov005_InitCoPaletteButtons). */
void Ov005_OnCoPaletteButtonConfirm(MenuButton* button) {
  Unk_ov005_0236bba8* work = data_ov005_0236bba8;
  int variant = button->id - 0xe0;

  work->variant = variant;
  if (variant != 8) {
    work->row0Variant = variant;
  }
  work->paletteConfirmTouched = 1;
}
