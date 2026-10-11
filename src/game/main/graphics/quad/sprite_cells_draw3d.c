#include "types.h"
#include "sdk/fx.h"
#include "sdk/gx.h"
#include "game/obj_size.h"

#define REG_MTX_SCALE ((volatile u32 *)0x0400046c)
#define TEXIMAGE_PARAM ((volatile u32 *)0x040004a8)
#define PLTT_BASE ((volatile u32 *)0x040004ac)
#define BEGIN_VTXS ((volatile u32 *)0x04000500)
#define END_VTXS ((volatile u32 *)0x04000504)

#define QUAD_FLIP_MASK 0x3000

int SizeFlag_ToIndex(int size);
int func_02013924(int size);
void func_0200ce3c(MtxFx43 *mtx);
void func_0201403c(int width, int height, int flip);
void Gx_SendObjSizedQuad(int width, int height, int flip);

extern MtxFx43 data_022a7da0;

/* Writes to MTX_TRANS (0x04000470) and MTX_SCALE (0x0400046c). Needed to match: all three
 * arguments are computed before the first register write. */
static inline void G3_Translate(fx32 x, fx32 y, fx32 z)
{
    *REG_MTX_TRANS = x;
    *REG_MTX_TRANS = y;
    *REG_MTX_TRANS = z;
}

static inline void G3_Scale(fx32 x, fx32 y, fx32 z)
{
    *REG_MTX_SCALE = x;
    *REG_MTX_SCALE = y;
    *REG_MTX_SCALE = z;
}

/* Draws a list of 2D OBJ cells as textured 16-colour 3D quads at pos, one
 * screen pixel per 1/128 unit. cells starts with a count, then three OAM
 * attribute halfwords per cell; tile and palette numbers count 32-byte blocks
 * from texBase and plttBase. scale, when set, scales each quad. With
 * billboard set (a guess at its use), each quad is anchored at its bottom
 * centre, turned by data_022a7da0 and drawn with func_0201403c's quads. */
void SpriteCells_Draw3d(const u16 *cells, const VecFx32 *pos, u32 texBase, u32 plttBase, const VecFx32 *scale, BOOL billboard)
{
    u32 count;
    int width;
    int height;
    int flip;
    int x;
    int y;
    u32 tile;
    u32 palette;
    u16 attr0;
    u16 attr1;
    u16 attr2;
    int sizeS;
    int sizeT;

    count = *cells++;

    *REG_MTX_PUSH = 0;
    *REG_MTX_MODE = 2;                 /* position and vector matrices */
    for (; count != 0; count--) {
        *REG_MTX_PUSH = 0;
        attr0 = cells[0];
        attr1 = cells[1];
        width = gObjSizes[attr0 >> 14][attr1 >> 14].width;
        height = gObjSizes[attr0 >> 14][attr1 >> 14].height;
        y = attr0 & 0xff;
        attr2 = cells[2];
        tile = attr2 & 0x3ff;
        flip = attr1 & QUAD_FLIP_MASK;
        palette = (attr2 & 0xf000) >> 12;
        x = attr1 & 0x1ff;
        if (x & 0x100) {
            x |= -0x200;
        }
        if (y & 0x80) {
            y |= ~0xff;
        }
        if (billboard) {
            int worldY = (y + height) * (6 * FX32_ONE) / 768;
            int worldX = (x + (width >> 1)) * (2 * FX32_ONE) / 256;
            G3_Translate(pos->x + worldX, -(pos->y + worldY), pos->z);
            func_0200ce3c(&data_022a7da0);
        } else {
            int worldY = y * (6 * FX32_ONE) / 768;
            int worldX = x * (2 * FX32_ONE) / 256;
            G3_Translate(pos->x + worldX, -(pos->y + worldY), pos->z);
        }
        sizeT = SizeFlag_ToIndex(height);
        sizeS = func_02013924(width);
        /* 16 colours, texture coordinates as given, colour 0 transparent. */
        *TEXIMAGE_PARAM = ((texBase + (tile << 5)) >> 3) | 0x4c000000 | (sizeS << 20) | (sizeT << 23) | 0x20000000;
        *PLTT_BASE = (plttBase + (palette << 5)) >> 4;
        if (scale != NULL) {
            G3_Scale(scale->x, scale->y, scale->z);
        }
        *BEGIN_VTXS = 1;
        if (billboard) {
            func_0201403c(width, height, flip);
        } else {
            Gx_SendObjSizedQuad(width, height, flip);
        }
        *END_VTXS = 0;
        *REG_MTX_POP = 1;
        cells += 3;
    }
    *REG_MTX_POP = 1;
}
