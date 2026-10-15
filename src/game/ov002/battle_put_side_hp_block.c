#include "types.h"
#include "game/menu_state.h"
#include "game/math.h"

#define BATTLE_FIELD(field) \
  ((u8*)&gMenuState + (u32) & ((MenuState*)0)->battle.field)

void Bg_PutTileBlock(u16* map, int x, int y, const u8* block, u16 tileAdd);

/* Puts the tile block for the side's starting HP (0 to 10 as shown on screen; 50 bytes each,
 * from gMenuState.battle.unk_510) into the tile map. */
void Battle_PutSideHpBlock(u16* tilemap, int side, int isAir) {
  BOOL left = (side == 0);
  int palette = left ? 8 : 9;
  int hp = BATTLE_FIELD(hpStart)[side];
  int shownHp;
  int row;
  int column;

  if (hp == 0) {
    shownHp = 0;
  } else {
    shownHp = HwDiv32(hp - 1, 10) + 1;
  }
  row = (isAir == 0) ? 1 : 19;
  column = left ? 9 : 17;
  Bg_PutTileBlock(tilemap + (column + row * 32), 0, 0,
                  BATTLE_FIELD(unk_510) + shownHp * 50, palette << 12);
}
