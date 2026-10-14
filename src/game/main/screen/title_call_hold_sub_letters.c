#include "types.h"
#include "game/oam_cells.h"
#include "game/screen_other_map.h"

void Sprite_Queue(int list, int x, int y, void* data, int attr, int tile);

extern const u8 data_0215bb60[];

/* Sprites on the sub screen, as in TitleCall_SubLettersGrowIn. */
#define LETTER_COUNT 8

/* Title call script step: draws the eight letter sprites on the sub screen
 * at full width, every frame. */
void TitleCall_HoldSubLetters(void) {
  Unk_027c0280* saved0280;
  int i;

  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  for (i = 0; i < LETTER_COUNT; i++) {
    Sprite_Queue(0, data_0215bb60[i] & 0x1ff, 320, &data_027c0018, 0, i * 32);
  }
  Screen_SetCurrentGfx(saved0280);
}
