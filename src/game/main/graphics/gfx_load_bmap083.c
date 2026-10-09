#include "types.h"
#include "game/Unk_02088d04.h"

void func_01ff9c48(const void *descriptor);
void func_02000fa4(void);
void Screen_SetBgMode(int mode);
void Screen_InitLayers(const void *config);
void func_020097c8(void);
void func_020099c8(void);
void func_02009ac0(void);
void FS_ChangeDir(const char *dir);
void File_UnpackTo(void *dest, const char *fileName);
void func_020030cc(const char *fileName, int fileOffset, int paletteOffset, int size);
void func_01ff8a00(u16 *entries, u16 byteCount, int addend);
BOOL func_01ff8ef8(int layer);
BOOL func_0200ac2c(int layer);
void Gfx_RequestLayerCopy(int layer);

extern Unk_027c0280 *data_027c0280;
extern u8 gBmap083Layers[];             /* layer config for Screen_InitLayers */
extern char data_02169c44[];           /* "/bmap" */
extern char gBmapFile083[];           /* "083" */
extern char gBmapFile084[];           /* "084" */
extern char gBmapFile07d[];           /* "07d" */
extern char gBmapFile07e[];           /* "07e" */
extern char gBmapFile07f[];           /* "07f" */
extern char gBmapFile080[];           /* "080" */
extern char gBmapFile081[];           /* "081" */
extern char gBmapFile082[];           /* "082" */
extern u32 gTextMapBytes[];            /* map bytes of a text BG by screen size */
extern u32 gAffineMapBytes[];            /* map bytes of a rotation/scaling BG by screen size */

/* Size in bytes of the tile map of a layer. func_01ff8ef8 and func_0200ac2c
 * tell the BG kind; the last kind uses 16-bit entries, so twice the bytes. */
#define LAYER_MAP_BYTES(layer, i)                                               \
    (func_01ff8ef8(i) ? gTextMapBytes[(layer)->screenSize]                      \
     : func_0200ac2c(i) ? gAffineMapBytes[(layer)->screenSize]                    \
                        : gAffineMapBytes[(layer)->screenSize] * 2)

/* Sets up the current screen from /bmap files 07d to 084: layer priorities
 * 0 to 3, alpha blending of layer 2 over layer 3, object tiles, the tiles,
 * maps and palettes of layers 2 and 3. Layer 2's map gets palette 8. Unknown
 * which screen this is. */
void Gfx_LoadBmap083(void)
{
    u16 *map;

    func_01ff9c48(func_02000fa4);
    Screen_SetBgMode(0);
    Screen_InitLayers(gBmap083Layers);
    func_020097c8();
    func_020099c8();
    func_02009ac0();
    data_027c0280->bgCnt[data_027c0280->layer0Bg].priority = 0;
    data_027c0280->bgCnt[data_027c0280->layer1Bg].priority = 1;
    data_027c0280->bgCnt[data_027c0280->layer2Bg].priority = 2;
    data_027c0280->bgCnt[data_027c0280->layer3Bg].priority = 3;
    data_027c0280->blendMode = 1;
    data_027c0280->blendEva = 16;
    data_027c0280->blendEvb = 16;
    data_027c0280->blendEvy = 0;
    data_027c0280->blendTopBg[data_027c0280->layer0Bg] = 0;
    data_027c0280->blendTopBg[data_027c0280->layer1Bg] = 0;
    data_027c0280->blendTopBg[data_027c0280->layer2Bg] = 1;
    data_027c0280->blendTopBg[data_027c0280->layer3Bg] = 0;
    data_027c0280->blendTopObj = 0;
    data_027c0280->blendBottomBg[data_027c0280->layer0Bg] = 0;
    data_027c0280->blendBottomBg[data_027c0280->layer1Bg] = 0;
    data_027c0280->blendBottomBg[data_027c0280->layer2Bg] = 0;
    data_027c0280->blendBottomBg[data_027c0280->layer3Bg] = 1;
    data_027c0280->blendBottomObj = 0;

    FS_ChangeDir(data_02169c44);
    File_UnpackTo(data_027c0280->objVram + 0x2000, gBmapFile083);
    func_020030cc(gBmapFile084, 0, 0x220, 32);
    File_UnpackTo((void *)GFX_LAYER(data_027c0280, 2)->charBase, gBmapFile07d);
    File_UnpackTo(GFX_LAYER(data_027c0280, 2)->map, gBmapFile07e);
    map = GFX_LAYER(data_027c0280, 2)->map;
    func_01ff8a00(map, LAYER_MAP_BYTES(GFX_LAYER(data_027c0280, 2), 2), 0x8000);

    func_020030cc(gBmapFile07f, 0, 0x100, 32);
    File_UnpackTo((void *)GFX_LAYER(data_027c0280, 3)->charBase, gBmapFile080);
    File_UnpackTo(GFX_LAYER(data_027c0280, 3)->map, gBmapFile081);
    map = GFX_LAYER(data_027c0280, 3)->map;
    func_01ff8a00(map, LAYER_MAP_BYTES(GFX_LAYER(data_027c0280, 3), 3), 0);

    func_020030cc(gBmapFile082, 0, 0, 256);
    Gfx_RequestLayerCopy(3);
    Gfx_RequestLayerCopy(2);
}
