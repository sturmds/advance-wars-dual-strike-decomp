#include "types.h"
#include "game/Unk_02088d04.h"

void MainLoop_SetFrameCallback(const void* descriptor);
void MainLoop_TaskFrame(void);
void Screen_SetBgMode(int mode);
void Screen_InitLayers(const void* config);
void Screen_ResetWindows(void);
void Screen_ResetBlend(void);
void Screen_ResetLayerScroll(void);
void FS_ChangeDir(const char* dir);
void File_UnpackTo(void* dest, const char* fileName);
void Palette_LoadFromFile(const char* fileName, int fileOffset,
                          int paletteOffset, int size);
void BgExtPalette_LoadFromFile(const char* fileName, int a, int b, int c);
void Gfx_RequestLayerCopy(int layer);

extern Unk_027c0280* data_027c0280;
extern u8 gBmap085Layers[];  /* layer config for Screen_InitLayers */
extern char data_02169c44[]; /* "/bmap" */
extern char gBmapFile085[];  /* "085" */
extern char gBmapFile086[];  /* "086" */
extern char gBmapFile087[];  /* "087" */
extern char gBmapFile088[];  /* "088" */
extern char gBmapFile08e[];  /* "08e" */
extern char gBmapFile08f[];  /* "08f" */
extern char gBmapFile090[];  /* "090" */

/* Sets up the current screen from /bmap files 085 to 090: layer priorities
 * 0 to 3, object tiles, the tiles and map of layer 3, two palettes, and
 * file 090 through BgExtPalette_LoadFromFile. Unknown which screen this is. */
void Gfx_LoadBmap085(void) {
  MainLoop_SetFrameCallback(MainLoop_TaskFrame);
  Screen_SetBgMode(0);
  Screen_InitLayers(gBmap085Layers);
  Screen_ResetWindows();
  Screen_ResetBlend();
  Screen_ResetLayerScroll();
  data_027c0280->bgCnt[data_027c0280->layer0Bg].priority = 0;
  data_027c0280->bgCnt[data_027c0280->layer1Bg].priority = 1;
  data_027c0280->bgCnt[data_027c0280->layer2Bg].priority = 2;
  data_027c0280->bgCnt[data_027c0280->layer3Bg].priority = 3;

  FS_ChangeDir(data_02169c44);
  File_UnpackTo(data_027c0280->objVram + 0xe00, gBmapFile085);
  Palette_LoadFromFile(gBmapFile086, 0, 0x240, 32);
  File_UnpackTo(data_027c0280->objVram + 0x2e00, gBmapFile087);
  Palette_LoadFromFile(gBmapFile088, 0, 0x280, 32);
  File_UnpackTo((void*)GFX_LAYER(data_027c0280, 3)->charBase, gBmapFile08e);
  File_UnpackTo(GFX_LAYER(data_027c0280, 3)->map, gBmapFile08f);
  BgExtPalette_LoadFromFile(gBmapFile090, 0, 0x200, 3);
  Gfx_RequestLayerCopy(3);
}
