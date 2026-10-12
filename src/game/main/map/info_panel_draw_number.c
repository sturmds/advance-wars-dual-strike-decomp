#include "types.h"
#include "game/info_panel.h"

extern u8 data_02166310[];

void Oam_PutSprite(int x, int y, void* shape, int tileAndPalette);

#define DIGIT_TILE_BASE 16
#define OAM_ATTR0_SEMI_TRANSPARENT 0x400 /* OAM attribute 0, object mode 1 */

/* The ones digit goes at x, the tens digit 7 pixels left of it unless it is 0. */
void InfoPanel_DrawNumber(u16 x, u16 y, s16 value) {
  s16 tens;

  Oam_PutSprite(x & 0x1ff, y | OAM_ATTR0_SEMI_TRANSPARENT, data_02166310,
                value % 10 + DIGIT_TILE_BASE);
  tens = value / 10;
  if (tens == 0) {
    return;
  }
  Oam_PutSprite((x - 7) & 0x1ff, y | OAM_ATTR0_SEMI_TRANSPARENT, data_02166310,
                tens + DIGIT_TILE_BASE);
}
