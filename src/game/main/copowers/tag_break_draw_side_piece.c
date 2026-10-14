#include "types.h"
#include "game/screen_other_map.h"
#include "game/oam_cells.h"

void Sprite_Queue(int list, int x, int y, void* data, int attr, int tile);
void func_02002598(int x, int y, int xBits, int yBits, int attr, int tile,
                   int list, int mirror);
void func_02002510(int x, int y, int xBits, int yBits, int attr, int tile,
                   int list, int mirror);

/* Draws one of the two side pieces of the tag power cut-in at (x, y). Screen
 * 1 gets a cap sprite and two sprite groups (at y and y + 192), screen 0 the
 * same groups 256 pixels higher, so the piece runs across both screens.
 * `side` (0 or 1) picks the palette, the tiles and the sprite list.
 * Needed to match: the saved screen pointers are block locals, the rest u32. */
void TagBreak_DrawSidePiece(int x, int y, int side) {
  u32 tileBottom;
  u32 tileTop;
  u32 attr;

  {
    Unk_027c0280* saved0280 = data_027c0280;
    Unk_02080440* saved027c;

    Screen_SetCurrentGfx(data_027c0288.unk_04);
    saved027c = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_04);
    attr = (side << 12) | 0x800;
    Sprite_Queue(side + 7, (x - 24) & 0x1ff, (y - 192) & 0xff, &data_027c00d8,
                 attr, side * 24 + 0x5b0);
    tileTop = side * 720 + 16;
    func_02002598(x, y, 0, 0, attr, tileTop, side + 7, 0);
    tileBottom = side * 720 + 448;
    func_02002510(x, y + 192, 0, 0, attr, tileBottom, side + 7, 0);
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);
  }
  {
    Unk_027c0280* saved0280 = data_027c0280;
    Unk_02080440* saved027c;

    Screen_SetCurrentGfx(data_027c0288.unk_00);
    saved027c = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_00);
    func_02002598(x, y - 256, 0, 0, attr, tileTop, side + 7, 0);
    func_02002510(x, y - 64, 0, 0, attr, tileBottom, side + 7, 0);
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);
  }
}
