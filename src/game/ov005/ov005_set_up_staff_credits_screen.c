#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/ov005_task.h"
#include "game/Unk_ov005_0235a30c.h"

void Screen_SetCurrent(void* state);
void Screen_SetCurrentGfx(void* state);
void Screen_SetBgMode(int mode);
void Screen_InitLayers(const void* config);
void FS_ChangeDir(const char* dir);
void File_UnpackTo(void* dest, const char* fileName);
void Palette_LoadFromFile(const char* fileName, int fileOffset,
                          int paletteOffset, int size);
void TileMap_AddToEntries(u16* entries, u16 byteCount, int addend);
BOOL Screen_IsTextBg(int layer);
BOOL Screen_IsAffineBg(int layer);
void Gfx_RequestLayerCopy(int layer);
void Ov005Screen_Init3dAndEntries(Unk_ov005_0235a30c* work);
void Ov005_UploadTileBuffer(Unk_ov005_0235a30c* work);
void Ov005Screen_LoadPalettes(Unk_ov005_0235a30c* work);
void Sound_FadeOutSong(void);
void Sound_StartStream(int strmNo);

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern Unk_027c0288 data_027c0288;
extern GfxLayer data_ov005_0236966c[];    /* main screen layer config */
extern GfxLayer gOv005CreditsSubLayers[]; /* sub screen layer config */
extern char data_ov005_0236988c[];        /* "/rikiishi" */
extern char gRikiishiFile05e[];
extern char gRikiishiFile05f[];
extern char gRikiishiFile060[];
extern char gRikiishiFile061[];
extern char gRikiishiFile062[];
extern char gRikiishiFile063[];
extern char data_ov005_023698bc[];
extern u32 data_ov005_02367f8c[]; /* map bytes of a text BG by screen size */
extern u32 data_ov005_02367f9c
    []; /* map bytes of a rotation/scaling BG by screen size */

/* First step of the staff credits screen (Ov005_UpdateStaffCreditsScreen
 * runs next). Sub screen: BG2-BG3 and OBJ on, BG2 alpha-blended over BG3 at
 * eva 0 (not visible yet); both layers come from /rikiishi files 05e to 063.
 * Main screen: BG0, BG2 and OBJ on, no blending. Fades out the song, starts
 * stream 0, sets both screens to black and starts at page 0. */
void Ov005_SetUpStaffCreditsScreen(Ov005Task* task) {
  Unk_ov005_0235a30c* work = task->work;
  Unk_027c0280* savedGfx;
  Unk_02080440* savedScreen;
  u16* map;

  savedGfx = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  savedScreen = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  Screen_SetBgMode(0);
  Screen_InitLayers(gOv005CreditsSubLayers);
  data_027c0280->dispCnt &= ~0x100;
  data_027c0280->dispCnt &= ~0x200;
  data_027c0280->dispCnt |= 0x400;
  data_027c0280->dispCnt |= 0x800;
  data_027c0280->dispCnt |= 0x1000;
  data_027c0280->bgCnt[data_027c0280->layer0Bg].priority = 0;
  data_027c0280->bgCnt[data_027c0280->layer1Bg].priority = 1;
  data_027c0280->bgCnt[data_027c0280->layer2Bg].priority = 2;
  data_027c0280->bgCnt[data_027c0280->layer3Bg].priority = 3;
  data_027c0280->dispCnt &= ~0x2000;
  data_027c0280->dispCnt &= ~0x4000;
  data_027c0280->dispCnt &= ~0x8000;
  data_027c0280->blendMode = BLEND_MODE_ALPHA;
  data_027c0280->blendEva = 0;
  data_027c0280->blendEvb = 16;
  data_027c0280->blendEvy = 0;
  data_027c0280->blendTopBackdrop = 0;
  data_027c0280->blendTopBg[data_027c0280->layer0Bg] = 0;
  data_027c0280->blendTopBg[data_027c0280->layer1Bg] = 0;
  data_027c0280->blendTopBg[data_027c0280->layer2Bg] = 1;
  data_027c0280->blendTopBg[data_027c0280->layer3Bg] = 0;
  data_027c0280->blendTopObj = 0;
  data_027c0280->blendBottomBackdrop = 0;
  data_027c0280->blendBottomBg[data_027c0280->layer0Bg] = 0;
  data_027c0280->blendBottomBg[data_027c0280->layer1Bg] = 0;
  data_027c0280->blendBottomBg[data_027c0280->layer2Bg] = 0;
  data_027c0280->blendBottomBg[data_027c0280->layer3Bg] = 1;
  data_027c0280->blendBottomObj = 0;
  data_027c0280->dispCnt |= 0x40;
  data_027c0280->unk_dd = 0;
  data_027c0280->bgScrollY[data_027c0280->layer0Bg] = 0;
  data_027c0280->bgScrollX[data_027c0280->layer0Bg] = 0;
  data_027c0280->bgScrollY[data_027c0280->layer1Bg] = 0;
  data_027c0280->bgScrollX[data_027c0280->layer1Bg] = 0;
  data_027c0280->bgScrollY[data_027c0280->layer2Bg] = 0;
  data_027c0280->bgScrollX[data_027c0280->layer2Bg] = 0;
  data_027c0280->bgScrollY[data_027c0280->layer3Bg] = 0;
  data_027c0280->bgScrollX[data_027c0280->layer3Bg] = 0;
  Screen_SetCurrent(savedScreen);
  Screen_SetCurrentGfx(savedGfx);

  savedGfx = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  savedScreen = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  FS_ChangeDir(data_ov005_0236988c);
  File_UnpackTo(
      data_027c0280->bgVram +
          (data_027c0280->bgCnt[data_027c0280->layer2Bg].charBase << 14),
      gRikiishiFile05e);
  File_UnpackTo(data_027c0280->layer2Map, gRikiishiFile05f);
  map = data_027c0280->layer2Map;
  TileMap_AddToEntries(
      map,
      Screen_IsTextBg(2)
          ? data_ov005_02367f8c[GFX_LAYER(data_027c0280, 2)->screenSize]
      : Screen_IsAffineBg(2)
          ? data_ov005_02367f9c[GFX_LAYER(data_027c0280, 2)->screenSize]
          : data_ov005_02367f9c[GFX_LAYER(data_027c0280, 2)->screenSize] * 2,
      0x8000);
  Palette_LoadFromFile(gRikiishiFile060, 0, 0x100, 32);
  Gfx_RequestLayerCopy(2);
  File_UnpackTo(
      data_027c0280->bgVram +
          (data_027c0280->bgCnt[data_027c0280->layer3Bg].charBase << 14),
      gRikiishiFile061);
  File_UnpackTo(data_027c0280->layer3Map, gRikiishiFile062);
  map = data_027c0280->layer3Map;
  TileMap_AddToEntries(
      map,
      Screen_IsTextBg(3)
          ? data_ov005_02367f8c[GFX_LAYER(data_027c0280, 3)->screenSize]
      : Screen_IsAffineBg(3)
          ? data_ov005_02367f9c[GFX_LAYER(data_027c0280, 3)->screenSize]
          : data_ov005_02367f9c[GFX_LAYER(data_027c0280, 3)->screenSize] * 2,
      0x9000);
  Palette_LoadFromFile(gRikiishiFile063, 0, 0x120, 0xa0);
  Gfx_RequestLayerCopy(3);
  Palette_LoadFromFile(data_ov005_023698bc, 0, 0x200, 32);
  Screen_SetCurrent(savedScreen);
  Screen_SetCurrentGfx(savedGfx);

  savedGfx = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  savedScreen = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  Screen_SetBgMode(0);
  Screen_InitLayers(data_ov005_0236966c);
  data_027c0280->bgCnt[data_027c0280->layer0Bg].priority = 0;
  data_027c0280->bgCnt[data_027c0280->layer1Bg].priority = 1;
  data_027c0280->bgCnt[data_027c0280->layer2Bg].priority = 2;
  data_027c0280->bgCnt[data_027c0280->layer3Bg].priority = 3;
  data_027c0280->dispCnt &= ~0x2000;
  data_027c0280->dispCnt &= ~0x4000;
  data_027c0280->dispCnt &= ~0x8000;
  data_027c0280->blendMode = 0;
  data_027c0280->blendEva = 16;
  data_027c0280->blendEvb = 0;
  data_027c0280->blendEvy = 0;
  data_027c0280->dispCnt |= 0x40;
  data_027c0280->unk_dd = 0;
  data_027c0280->bgScrollY[data_027c0280->layer0Bg] = 0;
  data_027c0280->bgScrollX[data_027c0280->layer0Bg] = 0;
  data_027c0280->bgScrollY[data_027c0280->layer1Bg] = 0;
  data_027c0280->bgScrollX[data_027c0280->layer1Bg] = 0;
  data_027c0280->bgScrollY[data_027c0280->layer2Bg] = 0;
  data_027c0280->bgScrollX[data_027c0280->layer2Bg] = 0;
  data_027c0280->bgScrollY[data_027c0280->layer3Bg] = 0;
  data_027c0280->bgScrollX[data_027c0280->layer3Bg] = 0;
  data_027c0280->dispCnt |= 0x100;
  data_027c0280->dispCnt &= ~0x200;
  data_027c0280->dispCnt |= 0x400;
  data_027c0280->dispCnt &= ~0x800;
  data_027c0280->dispCnt |= 0x1000;
  Screen_SetCurrent(savedScreen);
  Screen_SetCurrentGfx(savedGfx);

  Ov005Screen_Init3dAndEntries(work);
  Ov005_UploadTileBuffer(work);
  Ov005Screen_LoadPalettes(work);

  savedGfx = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  savedScreen = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  Palette_LoadFromFile(data_ov005_023698bc, 0, 0x200, 32);
  Screen_SetCurrent(savedScreen);
  Screen_SetCurrentGfx(savedGfx);

  Sound_FadeOutSong();
  Sound_StartStream(0);

  savedGfx = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  savedScreen = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  data_027c0280->masterBrightness = -16;
  Screen_SetCurrent(savedScreen);
  Screen_SetCurrentGfx(savedGfx);

  savedGfx = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  savedScreen = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  data_027c0280->masterBrightness = -16;
  Screen_SetCurrent(savedScreen);
  Screen_SetCurrentGfx(savedGfx);

  work->pageIndex = 0;
  task->step = 0;
}
