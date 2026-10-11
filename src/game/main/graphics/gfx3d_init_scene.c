#include "types.h"

#define GXFIFO_MTX_PUSH      (*(volatile u32 *)0x04000444)
#define GXFIFO_MTX_MODE      (*(volatile u32 *)0x04000440)
#define GXFIFO_MTX_IDENTITY  (*(volatile u32 *)0x04000454)
#define GXFIFO_TEXIMAGE_PARAM (*(volatile u32 *)0x04000484)
#define GXFIFO_POLYGON_ATTR  (*(volatile u32 *)0x040004a4)

#define GEOM_TARGET_TEXCOORD   3
#define GEOM_TARGET_POS_AND_DIR 2

extern u8 data_02159168[];
extern u8 data_0215915c[];
extern u8 data_0229c678[];

void G3X_Reset(void);
void func_0210bbd8(void *a, void *b, void *c, int d, int e);
void Gfx3d_SetDefaultLight(void);

/* Starts the 3D engine, then queues the default light, an identity texture
 * matrix and the default polygon attributes. */
void Gfx3d_InitScene(void)
{
    G3X_Reset();
    func_0210bbd8(data_02159168, data_0215915c, data_0229c678, 1, 0);
    Gfx3d_SetDefaultLight();
    GXFIFO_MTX_PUSH = 0;
    GXFIFO_MTX_MODE = GEOM_TARGET_TEXCOORD;
    GXFIFO_MTX_IDENTITY = 0;
    GXFIFO_MTX_MODE = GEOM_TARGET_POS_AND_DIR;
    GXFIFO_TEXIMAGE_PARAM = 0x20000000;
    GXFIFO_POLYGON_ATTR = 0x1f0081;
}
