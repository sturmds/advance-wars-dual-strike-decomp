#include "types.h"
#include "game/keypad.h"
#include "game/menu_button.h"
#include "game/Unk_ov005_0236bba8.h"

int Co_GetRank(u32 co);

extern Unk_ov005_0236bba8* data_ov005_0236bba8;

/* Moves the cursor over the CO's palette variant buttons with the d-pad.
 * Left and right wrap around the 8 variants of row 0, and the last one
 * there is kept in row0Variant. Down goes to the ninth variant (row 1), up
 * comes back; both need CO rank 10. */
void Ov005_MoveCoPaletteCursor(void) {
  Unk_ov005_0236bba8* work;
  int variant;
  u16 keys;
  int vertical;
  s16 col;
  s16 row;
  int horizontal;

  work = data_ov005_0236bba8;
  keys = gKeypad->repeat;

  vertical = 0;
  if (keys & PAD_KEY_UP) {
    vertical = 1;
  } else if (keys & PAD_KEY_DOWN) {
    vertical = 2;
  }
  horizontal = 0;
  if (keys & PAD_KEY_LEFT) {
    horizontal = 1;
  } else if (keys & PAD_KEY_RIGHT) {
    horizontal = 2;
  }

  if (Co_GetRank(work->coId) < 10) {
    vertical = 0;
  }
  variant = work->variant;
  col = work->paletteButtons[variant].col;
  row = work->paletteButtons[variant].row;

  if (vertical == 0 && row == 0) {
    if (horizontal == 1) {
      if (col > 0) {
        variant--;
      } else {
        variant += 7;
      }
    } else if (horizontal == 2) {
      if (col < 7) {
        variant++;
      } else {
        variant -= 7;
      }
    }
    work->row0Variant = variant;
  } else if (vertical == 1) {
    if (row == 1) {
      variant = work->row0Variant;
    }
  } else if (row == 0) {
    variant = 8;
  }

  MenuButton_Select(work->paletteButtons[variant].buttonId);
  work->variant = variant;
}
