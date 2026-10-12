#include "types.h"
#include "game/menu_state.h"

extern MenuState gMenuState;
#define BATTLE_FIELD(field) \
  ((u8*)&gMenuState + (u32) & ((MenuState*)0)->battle.field)
void Bg_PutTileBlock(u16* map, int x, int y, const u8* block, u16 tileAdd);

void func_ov002_0235e608(u16* tilemap, int side, int isAir) {
  BOOL left = (side == 0);
  int tile = left ? 0x16c : 0x18a;
  int palette = left ? 10 : 11;
  int row = (isAir == 0) ? 0 : 18;
  int column = left ? 0 : 27;

  Bg_PutTileBlock(tilemap + (column + row * 32), 0, 0,
                  BATTLE_FIELD(unk_750) + side * 62, tile | (palette << 12));
}
