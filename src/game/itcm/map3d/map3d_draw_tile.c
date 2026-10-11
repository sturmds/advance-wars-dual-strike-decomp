#include "types.h"
#include "sdk/fx.h"
#include "sdk/gx.h"
#include "game/Unk_022a7c5c.h"

void Gx_SendQuad_02012a88(void);
void Gx_SendQuad_02012cc0(void);
void Gx_SendQuad_02012ba4(void);
void Gx_SendQuad_020130a0(void);
void Gx_SendPlainQuad_02012ddc(void);
void Gx_SendPlainQuad_02012fb4(void);
void Gx_SendPlainQuad_02012ec8(void);
void Gx_SendPlainQuad_020132d8(void);

#define GEOM_TARGET_POS_AND_DIR 2
#define GEOM_TARGET_TEXCOORD         3

/* Draws one 16x16 16-colour map tile with the current matrix; pos is not
 * read. tile is a byte offset from data_022a7c5c->unk_00, palette a 32-byte
 * block from texPaletteOffset. shaded sends vertex colours with polygon
 * mode 2. reverse sends the four texcoords in reverse order, swapPairs
 * swaps the first two and the last two. */
void Map3d_DrawTile(VecFx32 *pos, int tile, int palette, int shaded, int reverse, int swapPairs)
{
    int polygonMode = shaded ? 2 : 0;

    *REG_MTX_PUSH = 0;
    *REG_MTX_MODE = GEOM_TARGET_TEXCOORD;
    *REG_MTX_IDENTITY = 0;
    *REG_MTX_MODE = GEOM_TARGET_POS_AND_DIR;
    /* 16 colours, 16x16, colour 0 transparent */
    *REG_TEXIMAGE_PARAM = 0x6c900000 | ((data_022a7c5c->unk_00 + tile) >> 3);
    *REG_PLTT_BASE = (data_022a7c5c->texPaletteOffset + palette * 32) >> 4;
    /* polygon id 0, opaque, front faces */
    *REG_POLYGON_ATTR = 0x001f0080 | (polygonMode << 4);
    *REG_BEGIN_VTXS = 1;
    if (shaded) {
        if (reverse) {
            if (swapPairs) {
                Gx_SendQuad_02012a88();
            } else {
                Gx_SendQuad_02012cc0();
            }
        } else {
            if (swapPairs) {
                Gx_SendQuad_02012ba4();
            } else {
                Gx_SendQuad_020130a0();
            }
        }
    } else {
        if (reverse) {
            if (swapPairs) {
                Gx_SendPlainQuad_02012ddc();
            } else {
                Gx_SendPlainQuad_02012fb4();
            }
        } else {
            if (swapPairs) {
                Gx_SendPlainQuad_02012ec8();
            } else {
                Gx_SendPlainQuad_020132d8();
            }
        }
    }
    *REG_END_VTXS = 0;
    *REG_MTX_POP = 1;
}
