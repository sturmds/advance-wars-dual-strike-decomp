#include "types.h"
#include "sdk/fx.h"
#include "game/move_path.h"
#include "game/Unk_022a7c5c.h"
#include "game/oam_cells.h"

#define GXFIFO_POLYGON_ATTR (*(volatile u32*)0x040004a4)

extern MovePath* gMovePath;

int MovePath_DirToNext(int i);
int MovePath_DirToPrev(int i);
void SpriteCells_Draw3d(const void* cells, const VecFx32* pos, u32 tileBase,
                        u32 unk28, int arg, int drawFlag);

/* Draws the move path arrow on every tile, last tile first, as 3D quads
 * (the screen MovePath_Draw sends here draws its map in 3D). Nothing is
 * drawn while the path is only the unit's own tile. */
void MovePath_DrawArrows3d(void) {
  s8 i;
  VecFx32 pos;
  int next;

  if (gMovePath->lastTile == 0) {
    return;
  }
  GXFIFO_POLYGON_ATTR = 0x001e4080; /* alpha 30, front faces */
  for (i = gMovePath->lastTile; i >= 0; i--) {
    /* pixels to fx32, 1.0 per 128 as in RangeSpread_Update */
    pos.x = (gMovePath->x[i] << 4) * 0x2000 / 256;
    pos.y = (gMovePath->y[i] << 4) * 0x6000 / 768;
    pos.z = 0;
    next = MovePath_DirToNext((u8)i);
    SpriteCells_Draw3d(
        &data_027c0040, &pos,
        data_022a7c5c->unk_48 +
            gMovePathArrowCells[next][MovePath_DirToPrev((u8)i)] * 32,
        data_022a7c5c->unk_4c, 0, 0);
  }
}
