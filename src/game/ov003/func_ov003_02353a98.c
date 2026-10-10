#include "types.h"

#define GXFIFO_MTX_PUSH      (*(volatile u32 *)0x04000444)
#define GXFIFO_MTX_MODE      (*(volatile u32 *)0x04000440)
#define GXFIFO_MTX_IDENTITY  (*(volatile u32 *)0x04000454)
#define GXFIFO_TEXIMAGE_PARAM (*(volatile u32 *)0x04000484)
#define GXFIFO_POLYGON_ATTR  (*(volatile u32 *)0x040004a4)
#define GEOM_TARGET_TEXCOORD   3
#define GEOM_TARGET_POS_AND_DIR 2
extern u8 data_ov003_02364a78[];
extern u8 data_ov003_02364a6c[];
extern u8 data_ov003_02365cf4[];
void func_0210b97c(void);
void func_0210bbd8(void *a, void *b, void *c, int d, int e);
void func_ov003_02353b34(void);

void func_ov003_02353a98(void)
{
    func_0210b97c();
    func_0210bbd8(data_ov003_02364a78, data_ov003_02364a6c, data_ov003_02365cf4, 1, 0);
    func_ov003_02353b34();
    GXFIFO_MTX_PUSH = 0;
    GXFIFO_MTX_MODE = GEOM_TARGET_TEXCOORD;
    GXFIFO_MTX_IDENTITY = 0;
    GXFIFO_MTX_MODE = GEOM_TARGET_POS_AND_DIR;
    GXFIFO_TEXIMAGE_PARAM = 0x20000000;
    GXFIFO_POLYGON_ATTR = 0x1f0081;
}
