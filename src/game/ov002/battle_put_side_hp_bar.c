#include "types.h"
#include "game/menu_state.h"

extern MenuState gMenuState;

#define BATTLE_FIELD(field) \
  ((u8*)&gMenuState + (u32) & ((MenuState*)0)->battle.field)

int HwDiv32(int num, int den);
void Battle_PutMeterRow(u16* tilemap, u8 x, u8 y, u16 tile, u8 palette, u8 fill,
                        u8 length, u8 upper, u8 lower);

/* Puts the four rows of the side's HP bar into the tile map. The bar has 34
 * steps, 8 or 9 in each row. The fill length is (starting HP + 2) / 3 + 1. */
void Battle_PutSideHpBar(u16* tilemap, int side, int isAir) {
  BOOL left = (side == 0);
  u16 tile = left ? 0x20 : 0xa8;
  int palette = left ? 8 : 9;
  int column = left ? 5 : 23;
  int y = ((isAir == 0) ? 0 : 18) + 1;
  int fill = HwDiv32(BATTLE_FIELD(hpStart)[side] + 2, 3) + 1;

  Battle_PutMeterRow(tilemap, column, y, tile, palette, fill, 8, 34, 26);
  Battle_PutMeterRow(tilemap, column, y + 1, tile + 32, palette, fill, 9, 26,
                     17);
  Battle_PutMeterRow(tilemap, column, y + 2, tile + 68, palette, fill, 9, 17,
                     8);
  Battle_PutMeterRow(tilemap, column, y + 3, tile + 104, palette, fill, 8, 8,
                     0);
}
