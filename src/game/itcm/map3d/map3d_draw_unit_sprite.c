#include "types.h"
#include "sdk/fx.h"
#include "sdk/gx.h"
#include "game/Unk_022a7c5c.h"

extern MtxFx43 data_022a7da0;

int func_01ffd854(
    void); /* sets the diffuse, ambient, specular and emission colours to white */
void func_0200ce3c(MtxFx43* mtx);
void Gx_SendQuad(void);
void Gx_SendQuad_02012794(void);
void Gx_SendPlainQuad_020128b0(void);
void Gx_SendPlainQuad_0201299c(void);

#define GEOM_TARGET_POS_AND_DIR 2
#define GEOM_TARGET_TEXCOORD 3

/* Draws one 32x32 16-colour unit sprite with the matrix data_022a7da0;
 * pos is not read. tile is a byte offset from data_022a7c5c->anim2Dst,
 * palette a 32-byte block from unk_34. flip sends the texcoords in
 * reverse order, shaded sends vertex colours with polygon mode 2 (toon or
 * highlight), translucent sets alpha 6 of 31. */
void Map3d_DrawUnitSprite(VecFx32* pos, int tile, int palette, int flip,
                          int shaded, int translucent) {
  int polygonMode = shaded ? 2 : 0;

  *REG_MTX_PUSH = 0;
  func_01ffd854();
  *REG_MTX_MODE = GEOM_TARGET_TEXCOORD;
  *REG_MTX_IDENTITY = 0;
  *REG_MTX_MODE = GEOM_TARGET_POS_AND_DIR;
  func_0200ce3c(&data_022a7da0);
  /* 16 colours, 32x32, colour 0 transparent */
  *REG_TEXIMAGE_PARAM =
      0x6d200000 | ((u32)(data_022a7c5c->anim2Dst + tile) >> 3);
  *REG_PLTT_BASE = (u32)(data_022a7c5c->unk_34 + palette * 32) >> 4;
  *REG_POLYGON_ATTR =
      (polygonMode << 4) | 0x80 | ((translucent ? 6 : 31) << 16);
  *REG_BEGIN_VTXS = 1;
  if (shaded) {
    if (flip) {
      Gx_SendQuad();
    } else {
      Gx_SendQuad_02012794();
    }
  } else {
    if (flip) {
      Gx_SendPlainQuad_020128b0();
    } else {
      Gx_SendPlainQuad_0201299c();
    }
  }
  *REG_END_VTXS = 0;
  *REG_MTX_POP = 1;
}
