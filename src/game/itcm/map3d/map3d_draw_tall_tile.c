#include "types.h"
#include "sdk/fx.h"
#include "sdk/gx.h"
#include "game/Unk_022a7c5c.h"

extern MtxFx43 data_022a7da0;

void func_0200ce3c(MtxFx43 *mtx);
void Gx_SendQuad_020131bc(void);
void Gx_SendPlainQuad_020133c4(void);

#define GEOM_TARGET_POS_AND_DIR 2
#define GEOM_TARGET_TEXCOORD         3

/* Draws one 16x32 16-colour map tile with the matrix data_022a7da0; pos is
 * not read. Map3d_DrawMapTile draws a 16x16 ground tile first, then this one.
 * tile is a byte offset from data_022a7c5c->unk_00, palette a 32-byte block
 * from texPaletteOffset. shaded sends vertex colours with polygon mode 2. */
void Map3d_DrawTallTile(VecFx32 *pos, int tile, int palette, int shaded)
{
    int polygonMode = shaded ? 2 : 0;

    *REG_MTX_PUSH = 0;
    *REG_MTX_MODE = GEOM_TARGET_TEXCOORD;
    *REG_MTX_IDENTITY = 0;
    *REG_MTX_MODE = GEOM_TARGET_POS_AND_DIR;
    func_0200ce3c(&data_022a7da0);
    /* 16 colours, 16x32, colour 0 transparent */
    *REG_TEXIMAGE_PARAM = 0x6d100000 | ((data_022a7c5c->unk_00 + tile) >> 3);
    *REG_PLTT_BASE = (data_022a7c5c->texPaletteOffset + palette * 32) >> 4;
    /* polygon id 0, opaque, front faces */
    *REG_POLYGON_ATTR = 0x001f0080 | (polygonMode << 4);
    *REG_BEGIN_VTXS = 1;
    if (shaded) {
        Gx_SendQuad_020131bc();
    } else {
        Gx_SendPlainQuad_020133c4();
    }
    *REG_END_VTXS = 0;
    *REG_MTX_POP = 1;
}
