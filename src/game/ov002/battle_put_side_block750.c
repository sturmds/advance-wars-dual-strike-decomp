#include "types.h"
#include "game/menu_state.h"

#define BATTLE_FIELD(field) \
  ((u8*)&gMenuState + (u32) & ((MenuState*)0)->battle.field)

void Bg_PutTileBlock(u16* map, int x, int y, const u8* block, u16 tileAdd);

/* Puts the side's 62-byte tile block (gMenuState.battle.unk_750) into the tile map: left
 * edge for side 0, column 27 for side 1, 18 rows down for an air unit. */
void Battle_PutSideBlock750(u16* tilemap, int side, int isAir) {
  BOOL left = (side == 0);
  int tile = left ? 0x130 : 0x14e;
  int palette = left ? 10 : 11;
  int row = (isAir == 0) ? 0 : 18;
  int column = left ? 0 : 27;

  Bg_PutTileBlock(tilemap + (column + row * 32), 0, 0,
                  BATTLE_FIELD(unk_750) + side * 62, tile | (palette << 12));
}
