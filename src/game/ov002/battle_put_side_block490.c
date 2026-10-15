#include "types.h"
#include "game/menu_state.h"

#define BATTLE_FIELD(field) \
  ((u8*)&gMenuState + (u32) & ((MenuState*)0)->battle.field)

void Bg_PutTileBlock(u16* map, int x, int y, const u8* block, u16 tileAdd);
void Battle_PutSideHpBar(u16* tilemap, int side, int isAir);

/* Puts the side's 50-byte tile block (gMenuState.battle.unk_490) into the tile map
 * like Battle_PutSideBlock750, then draws the HP bar with Battle_PutSideHpBar. */
void Battle_PutSideBlock490(u16* tilemap, int side, int isAir) {
  BOOL left = (side == 0);
  int palette = left ? 8 : 9;
  int row = (isAir == 0) ? 0 : 18;
  int column = left ? 5 : 23;

  Bg_PutTileBlock(tilemap + (column + row * 32), 0, 0,
                  BATTLE_FIELD(unk_490) + side * 50, palette << 12);
  Battle_PutSideHpBar(tilemap, side, isAir);
}
