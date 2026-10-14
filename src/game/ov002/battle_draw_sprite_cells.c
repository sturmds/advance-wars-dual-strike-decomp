#include "types.h"
#include "game/menu_state.h"
#include "game/BattleSideData.h"

#define REG_POLYGON_ATTR ((volatile u32*)0x040004a4)
#define REG_MTX_PUSH ((volatile u32*)0x04000444)
#define REG_MTX_POP ((volatile u32*)0x04000448)
#define REG_MTX_TRANS ((volatile u32*)0x04000470)

extern MenuState gMenuState;
extern s16 data_0214d4c4[];

/* Needed to match: the side's offset into gMenuState is added at run time. SIDE_DATA_AGAIN
 * is the same pointer written another way, so that the compiler loads the whole chain again
 * where the original does. */
#define SIDE_STATE(side) ((MenuState*)((u8*)&gMenuState + (side) * 8))
#define SIDE_DATA(side) \
  (((BattleSideData**)((u8*)&gMenuState + (side) * 4))[0x27c / 4])
#define SIDE_DATA_AGAIN(side) \
  (((BattleSideData**)&gMenuState + (side))[0x27c / 4])

void G3_RotX(int sin, int cos);
BOOL Battle_IsSideAir(int side);
int Battle_GetTexHeight(u32 index);
int Battle_GetTexWidth(u32 index);
void Battle_DrawTexturedQuad(int x, int y, int z, int width, int height,
                             u32 sizeS, u32 sizeT, u32 texAddr, u32 palAddr,
                             int flipX, int flipY);

/* Draws one battle side's sprite frame as textured quads. cells starts with a cell count, then
 * three halfwords per cell, laid out like OBJ attributes: x (9 bits, signed), flips and the
 * texture width index; y (8 bits, signed) and the height index; z (bits 10-11), the texture
 * (bits 0-9, added to texBase) and the palette (bits 12-15, added to palBase). Side 0 is
 * drawn mirrored. Before drawing it rotates by minus the side's sway angle around (0, y, z),
 * against the turn Battle_DrawScene gives the side. Scales are 8.8 fixed point. */
void Battle_DrawSpriteCells(u16* cells, int x, int y, int z, int offX, int offY,
                            int offZ, int scaleX, int scaleY, u32 texBase,
                            u32 palBase, int mirror, int side) {
  int idx;
  u32 sizeS;
  u32 sizeT;
  int count;
  int cellY;
  int cellZ;
  int width;
  u32 tex;
  u32 pal;
  u16 word;
  int flipX;
  int cellX;
  int flipY;
  int i;
  int scaledX;
  int height;

  count = *cells;

  if (side == 0) {
    offX = -offX;
    x = -x;
    mirror = (mirror + 1) & 1;
  }
  *REG_POLYGON_ATTR = 0x1f0081; /* light 0, front faces, opaque */
  *REG_MTX_PUSH = 0;
  *REG_MTX_TRANS = 0;
  *REG_MTX_TRANS = y;
  *REG_MTX_TRANS = z;
  idx =
      (-((SIDE_STATE(side)->battle.sway[0].angle + 0x600) & 0xffff) & 0xffff) >>
      4;
  cells++;
  G3_RotX(data_0214d4c4[idx * 2], data_0214d4c4[idx * 2 + 1]);
  *REG_MTX_TRANS = 0;
  *REG_MTX_TRANS = -y;
  *REG_MTX_TRANS = -z;
  if (Battle_IsSideAir(side)) {
    y -= 64;
  }
  /* A side with unk_48 set moves the sprite by its shift and a quarter of its scale. */
  if (SIDE_DATA(side)->unk_48 == 1) {
    y -= ((u16)SIDE_DATA(side)->layout->fighter.scale << 6) >> 8;
    if (side == 0) {
      x -= SIDE_DATA(side)->layout->fighter.shift.x;
    } else {
      x += SIDE_DATA(side)->layout->fighter.shift.x;
    }
    y += SIDE_DATA_AGAIN(side)->layout->fighter.shift.y;
    z += SIDE_DATA_AGAIN(side)->layout->fighter.shift.z;
  }
  x += offX;
  y += offY;
  z += offZ;
  for (i = 0; i < count; i++) {
    word = cells[0];
    if (word & 0x100) {
      cellX = (word & 0xff) - 0x100;
    } else {
      cellX = word & 0xff;
    }
    flipX = (word >> 10) & 1;
    flipY = (word >> 11) & 1;
    sizeS = word >> 12;
    word = cells[1];
    if (word & 0x80) {
      cellY = (word & 0x7f) - 0x80;
    } else {
      cellY = word & 0x7f;
    }
    sizeT = word >> 12;
    word = cells[2];
    cells += 3;
    cellZ = (word >> 10) & 3;
    tex = word & 0x3ff;
    pal = word >> 12;
    if (mirror) {
      /* Mirror the cell around x = 0: its right edge becomes its left. */
      cellX = -cellX;
      cellX -= Battle_GetTexWidth(sizeS);
      flipX = (flipX + 1) & 1;
    }
    /* Needed to match: a plain scaledX = cellX * scaleX moves the multiply to the call. */
    scaledX = cellX;
    scaledX *= scaleX;
    width = (Battle_GetTexWidth(sizeS) * scaleX) >> 8;
    height = (Battle_GetTexHeight(sizeT) * scaleY) >> 8;
    Battle_DrawTexturedQuad(x + (scaledX >> 8), y + ((cellY * scaleY) >> 8),
                            z + cellZ, width, height, sizeS, sizeT,
                            tex + texBase, pal + palBase, flipX, flipY);
  }
  *REG_MTX_POP = 1;
}
