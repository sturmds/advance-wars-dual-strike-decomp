#include "types.h"
#include "sdk/fx.h"
#include "sdk/gx.h"
#include "game/Unk_022a7c5c.h"

extern MtxFx44 data_022a7c6c;
extern MtxFx44 data_022a7dc4;

void func_0210b5d4(MtxFx44 *mtx); /* loads a 4x4 matrix into the current matrix */
/* Projects (x, y, z) through mtx to a screen position in out. */
void func_0200c3d8(int x, int y, int z, MtxFx44 *mtx, VecFx32 *out);
void Gx_SendPlainQuad_0201258c(void);

/* Writes x, y, z to the geometry engine's MTX_TRANS register (0x04000470). */
static inline void G3_Translate(fx32 x, fx32 y, fx32 z)
{
    *REG_MTX_TRANS = x;
    *REG_MTX_TRANS = y;
    *REG_MTX_TRANS = z;
}

#define GEOM_TARGET_PROJ      0
#define GEOM_TARGET_POS_AND_DIR 2
#define GEOM_TARGET_TEXCOORD         3

/* Draws one 8x8 16-colour icon at the screen position of pos, with an
 * projection matrix data_022a7c6c. tile is a byte offset from
 * data_022a7c5c->unk_38, palette a 32-byte block from unk_3c. Draws
 * nothing for tile -1 or hidden 1. */
void Map3d_DrawIcon(VecFx32 *pos, int tile, int palette, int hidden)
{
    VecFx32 screen;

    if (tile == -1) {
        return;
    }
    if (hidden == 1) {
        return;
    }
    *REG_MTX_PUSH = 0;
    *REG_MTX_MODE = GEOM_TARGET_PROJ;
    *REG_MTX_PUSH = 0;
    func_0210b5d4(&data_022a7c6c);
    *REG_MTX_MODE = GEOM_TARGET_TEXCOORD;
    *REG_MTX_IDENTITY = 0;
    *REG_MTX_MODE = GEOM_TARGET_POS_AND_DIR;
    *REG_MTX_IDENTITY = 0;
    func_0200c3d8(pos->x, pos->y, pos->z, &data_022a7dc4, &screen);
    G3_Translate(screen.x, screen.y, screen.z);
    /* 16 colours, 8x8, colour 0 transparent */
    *REG_TEXIMAGE_PARAM = ((data_022a7c5c->unk_38 + tile) >> 3) | 0x6c000000;
    *REG_PLTT_BASE = (data_022a7c5c->unk_3c + palette * 32) >> 4;
    /* polygon id 0, opaque, front faces */
    *REG_POLYGON_ATTR = 0x001f0080;
    *REG_BEGIN_VTXS = 1;
    Gx_SendPlainQuad_0201258c();
    *REG_END_VTXS = 0;
    *REG_MTX_MODE = GEOM_TARGET_PROJ;
    *REG_MTX_POP = 1;
    *REG_MTX_MODE = GEOM_TARGET_POS_AND_DIR;
    *REG_MTX_POP = 1;
}
