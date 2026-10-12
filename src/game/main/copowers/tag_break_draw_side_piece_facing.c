#include "types.h"
#include "game/screen_other_map.h"

void Sprite_Queue(int list, int x, int y, void* data, int attr, int tile);
void func_02002598(int x, int y, int xBits, int yBits, int attr, int tile,
                   int list, int mirror);
void func_02002510(int x, int y, int xBits, int yBits, int attr, int tile,
                   int list, int mirror);

extern u8 data_027c00d8;

#define OBJ_X_FLIP 0x1000 /* OAM attribute 1 */

/* Like TagBreak_DrawSidePiece, but the piece of side 0 is flipped
 * horizontally (its cap sprite drawn at x + 24, its sprite groups flipped), so
 * the two sides face each other.
 * Needed to match: the saved screen pointers are block locals, the rest u32;
 * attr is set in both arms. */
void TagBreak_DrawSidePieceFacing(int x, int y, int side) {
  u32 tileBottom;
  u32 tileTop;
  u32 attr;
  u32 flipX;

  {
    Unk_027c0280* saved0280 = data_027c0280;
    Unk_02080440* saved027c;

    Screen_SetCurrentGfx(data_027c0288.unk_04);
    saved027c = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_04);
    if (side == 1) {
      attr = (side << 12) | 0x800;
      Sprite_Queue(side + 7, (x - 24) & 0x1ff, (y - 192) & 0xff, &data_027c00d8,
                   attr, side * 24 + 0x5b0);
    } else {
      attr = (side << 12) | 0x800;
      Sprite_Queue(side + 7, ((x + 24) & 0x1ff) | OBJ_X_FLIP, (y - 192) & 0xff,
                   &data_027c00d8, attr, side * 24 + 0x5b0);
    }
    flipX = 1 - (side & 1);
    tileTop = side * 720 + 16;
    func_02002598(x, y, 0, 0, attr, tileTop, side + 7, flipX);
    tileBottom = side * 720 + 448;
    func_02002510(x, y + 192, 0, 0, attr, tileBottom, side + 7, flipX);
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);
  }
  {
    Unk_027c0280* saved0280 = data_027c0280;
    Unk_02080440* saved027c;

    Screen_SetCurrentGfx(data_027c0288.unk_00);
    saved027c = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_00);
    func_02002598(x, y - 256, 0, 0, attr, tileTop, side + 7, flipX);
    func_02002510(x, y - 64, 0, 0, attr, tileBottom, side + 7, flipX);
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);
  }
}
