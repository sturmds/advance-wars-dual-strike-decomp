#include "types.h"
#include "sdk/gx.h"
#include "game/Unk_022a7c5c.h"

#define GEOM_TARGET_TEXCOORD 3
#define GEOM_TARGET_POS_AND_DIR 2
#define TEXIMAGE_PARAM ((volatile u32 *)0x040004a8)
#define PLTT_BASE ((volatile u32 *)0x040004ac)
#define POLYGON_ATTR ((volatile u32 *)0x040004a4)
#define BEGIN_VTXS ((volatile u32 *)0x04000500)
#define END_VTXS ((volatile u32 *)0x04000504)

void Gx_SendQuad_020134b0(void);
void Gx_SendPlainQuad_020136e8(void);

/* Draws one quad of the 3D map from the vertex table of Gx_SendQuad_020134b0.
 * Texture image at data_022a7c5c->unk_00 + texOffset, palette number counts
 * 32-byte blocks. shaded: polygon mode 2 and per-vertex colours, otherwise
 * plain vertices. unused is not read. */
void MapTile3d_DrawQuadB(int unused, int texOffset, int palette, BOOL shaded)
{
    int mode = shaded ? 2 : 0;

    *REG_MTX_PUSH = 0;
    *REG_MTX_MODE = GEOM_TARGET_TEXCOORD;
    *REG_MTX_IDENTITY = 0;
    *REG_MTX_MODE = GEOM_TARGET_POS_AND_DIR;
    *TEXIMAGE_PARAM = 0x6db00000 | ((data_022a7c5c->unk_00 + texOffset) >> 3);
    *PLTT_BASE = (data_022a7c5c->texPaletteOffset + (palette << 5)) >> 4;
    *POLYGON_ATTR = 0x001f0080 | (mode << 4);
    *BEGIN_VTXS = 1;
    if (shaded) {
        Gx_SendQuad_020134b0();
    } else {
        Gx_SendPlainQuad_020136e8();
    }
    *END_VTXS = 0;
    *REG_MTX_POP = 1;
}
