#include "types.h"
#include "game/Unk_022a7c5c.h"

extern const u8 gArmyPalettes[][32];
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

void GX_BeginLoadTex(void); /* begins a texture image load */
void GX_LoadTex(const void* src, u32 offset,
                u32 size); /* copies to texture image VRAM */
void GX_EndLoadTex(void);  /* ends the load */
void FS_ChangeDir(const char* dir);
void Gfx_LoadTexImageFile(const char* fileName, u32 offset);
void Gfx_LoadTexPaletteFile(const char* fileName, u32 offset, u32 size);
void Gfx_LoadTexPalette(const void* src, u32 offset, u32 size);
void File_UnpackTo(void* dest, const char* fileName);

extern u32 gMap3dClearBlock[]; /* MAP3D_CLEAR_BLOCK_SIZE zero bytes */

/* Makes gfx the current 3D map graphics object, clears three of its texture
 * areas (16 KB at unk_68, 2 KB each at unk_5c and unk_60) and loads the
 * /bmap textures and palettes, with the army palettes, into texture VRAM.
 * File 01d also goes to the frames of the second map animation. */
void Map3d_LoadTextures(Unk_022a7c5c* gfx) {
  int offset;

  data_022a7c5c = gfx;
  GX_BeginLoadTex();
  for (offset = 0; offset < MAP3D_TEX_AREA_SIZE;
       offset += MAP3D_CLEAR_BLOCK_SIZE) {
    GX_LoadTex(gMap3dClearBlock, (u32)gfx->unk_68 + offset,
               MAP3D_CLEAR_BLOCK_SIZE);
  }
  for (offset = 0; offset < MAP3D_SLOT_TEX_AREA_SIZE;
       offset += MAP3D_CLEAR_BLOCK_SIZE) {
    GX_LoadTex(gMap3dClearBlock, gfx->unk_5c + offset, MAP3D_CLEAR_BLOCK_SIZE);
    GX_LoadTex(gMap3dClearBlock, gfx->unk_60 + offset, MAP3D_CLEAR_BLOCK_SIZE);
  }
  GX_EndLoadTex();
  FS_ChangeDir(data_0216a45c);
  Gfx_LoadTexImageFile(data_0216a464, (u32)gfx->anim2Dst);
  File_UnpackTo(gfx->anim2Frames[0], data_0216a464);
  File_UnpackTo(gfx->anim2Frames[1], data_0216a468);
  File_UnpackTo(gfx->anim2Frames[2], data_0216a46c);
  Gfx_LoadTexPalette(gArmyPalettes, (u32)gfx->unk_34,
                     sizeof(gArmyPalettes[0]) * 10);
  Gfx_LoadTexImageFile(data_0216a470, gfx->unk_38);
  Gfx_LoadTexImageFile(data_0216a474, gfx->unk_40);
  Gfx_LoadTexPaletteFile(data_0216a478, (u32)gfx->unk_44, 32);
  Gfx_LoadTexImageFile(data_0216a47c, gfx->unk_48);
  Gfx_LoadTexPaletteFile(data_0216a480, gfx->unk_4c, 32);
  Gfx_LoadTexImageFile(data_0216a484, 0x680);
  Gfx_LoadTexPaletteFile(data_0216a488, 0, 32);
}
