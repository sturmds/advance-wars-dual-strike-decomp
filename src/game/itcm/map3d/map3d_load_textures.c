#include "types.h"
#include "game/Unk_022a7c5c.h"

extern u16 gArmyPalettes[][16];
extern char data_0216a45c[]; /* "/bmap" */
extern char data_0216a464[]; /* "01d" */
extern char data_0216a468[]; /* "01e" */
extern char data_0216a46c[]; /* "01f" */
extern char data_0216a470[]; /* "01c" */
extern char data_0216a474[]; /* "027" */
extern char data_0216a478[]; /* "028" */
extern char data_0216a47c[]; /* "029" */
extern char data_0216a480[]; /* "02a" */
extern char data_0216a484[]; /* "02b" */
extern char data_0216a488[]; /* "02c" */

void GX_BeginLoadTex(void);                                   /* begins a texture image load */
void func_0210c9a0(const void *src, u32 offset, u32 size); /* copies to texture image VRAM */
void GX_EndLoadTex(void);                                   /* ends the load */
void FS_ChangeDir(const char *dir);
void Gfx_LoadTexImageFile(const char *fileName, u32 offset);
void Gfx_LoadTexPaletteFile(const char *fileName, u32 offset, u32 size);
void Gfx_LoadTexPalette(const void *src, u32 offset, u32 size);
void File_UnpackTo(void *dest, const char *fileName);

/* 32 zero bytes in ITCM, right after RangeMap_FillMoves. The original
 * reads them through a plain address with no relocation, so it stays a
 * number here. */
#define ITCM_ZERO_BLOCK ((const void *)0x01ffb95c)

/* Makes gfx the current 3D map graphics object, clears three of its texture
 * areas (16 KB at unk_68, 2 KB each at unk_5c and unk_60) and loads the
 * /bmap textures and palettes, with the army palettes, into texture VRAM.
 * File 01d also goes to the frames of the second map animation. */
void Map3d_LoadTextures(Unk_022a7c5c *gfx)
{
    int offset;

    data_022a7c5c = gfx;
    GX_BeginLoadTex();
    for (offset = 0; offset < 0x4000; offset += 32) {
        func_0210c9a0(ITCM_ZERO_BLOCK, (u32)gfx->unk_68 + offset, 32);
    }
    for (offset = 0; offset < 0x800; offset += 32) {
        func_0210c9a0(ITCM_ZERO_BLOCK, gfx->unk_5c + offset, 32);
        func_0210c9a0(ITCM_ZERO_BLOCK, gfx->unk_60 + offset, 32);
    }
    GX_EndLoadTex();
    FS_ChangeDir(data_0216a45c);
    Gfx_LoadTexImageFile(data_0216a464, (u32)gfx->anim2Dst);
    File_UnpackTo(gfx->anim2Frames[0], data_0216a464);
    File_UnpackTo(gfx->anim2Frames[1], data_0216a468);
    File_UnpackTo(gfx->anim2Frames[2], data_0216a46c);
    Gfx_LoadTexPalette(gArmyPalettes, (u32)gfx->unk_34, sizeof(gArmyPalettes[0]) * 10);
    Gfx_LoadTexImageFile(data_0216a470, gfx->unk_38);
    Gfx_LoadTexImageFile(data_0216a474, gfx->unk_40);
    Gfx_LoadTexPaletteFile(data_0216a478, (u32)gfx->unk_44, 32);
    Gfx_LoadTexImageFile(data_0216a47c, gfx->unk_48);
    Gfx_LoadTexPaletteFile(data_0216a480, gfx->unk_4c, 32);
    Gfx_LoadTexImageFile(data_0216a484, 0x680);
    Gfx_LoadTexPaletteFile(data_0216a488, 0, 32);
}
