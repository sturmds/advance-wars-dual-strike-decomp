#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/ov005_task.h"
#include "game/task.h"
#include "game/Unk_ov005_0236bb84.h"

void Screen_SetCurrent(void* state);
void Screen_SetCurrentGfx(void* state);
void Screen_SetBgMode(int mode);
void Screen_InitLayers(const void* config);
void FS_ChangeDir(const char* dir);
void File_UnpackTo(void* dest, const char* fileName);
void Palette_LoadFromFile(const char* fileName, int fileOffset,
                          int paletteOffset, int size);
void Palette_Load(const void* src, u32 offset, u32 size);
void Font_LoadDigits(int baseTile);
void Mem_Zero32(u32 value, void* dest, u32 size);
void TileMap_AddToEntries(u16* entries, u16 byteCount, int addend);
BOOL Screen_IsTextBg(int layer);
BOOL Screen_IsAffineBg(int layer);
void Gfx_RequestLayerCopy(int layer);
Task* Sound_StartSongSwitch(
    int song, int arg); /* task: fade out the song, then start `song` */

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern Unk_027c0288 data_027c0288;
extern u8 data_ov005_02368378[];    /* main screen layer config */
extern u8 gOv005BlendInSubLayers[]; /* sub screen layer config */
extern u8 data_021388f0[];
extern char data_ov005_02368a08[]; /* "/rikiishi" */
extern char gRikiishiFile016[];
extern char gRikiishiFile018[];
extern char gRikiishiFile01b[];
extern char gRikiishiFile013[];
extern char gRikiishiFile014[];
extern char gRikiishiFile015[];
extern char gRikiishiFile01f[];
extern char gRikiishiFile020[];
extern char gRikiishiFile021[];
extern u32 data_ov005_02367f2c[]; /* map bytes of a text BG by screen size */
extern u32 data_ov005_02367f3c
    []; /* map bytes of a rotation/scaling BG by screen size */

/* Screen setup step of task script data_ov005_023682e0; the next step,
 * Ov005_UpdateBlendEventScriptScreen, fades the sub screen's layer 0 in.
 * Sub screen: BG0 and BG2 on, OBJ off, both layers and their palettes from
 * /rikiishi files 013 to 01b, layer 0 alpha-blended over layer 2 with
 * weights 0 and 16 (so layer 0 starts hidden) and scrolled 64 pixels.
 * Main screen: BG0, BG2 and OBJ on, layer 2 from files 01f to 021 scrolled
 * 8 pixels, the digit font. Starts song 54. */
void Ov005_SetupBlendInScreensWithSong(Ov005Task* task) {
  Unk_ov005_0236bb84* work;
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;
  u16* map;

  work = task->work;
  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  Screen_SetBgMode(0);
  Screen_InitLayers(data_ov005_02368378);
  data_027c0280->dispCnt |= 0x100;
  data_027c0280->dispCnt &= ~0x200;
  data_027c0280->dispCnt |= 0x400;
  data_027c0280->dispCnt &= ~0x800;
  data_027c0280->dispCnt |= 0x1000;
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);

  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  Screen_SetBgMode(0);
  Screen_InitLayers(gOv005BlendInSubLayers);
  data_027c0280->dispCnt |= 0x100;
  data_027c0280->dispCnt &= ~0x200;
  data_027c0280->dispCnt |= 0x400;
  data_027c0280->dispCnt &= ~0x800;
  data_027c0280->dispCnt &= ~0x1000;
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);

  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
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
  data_027c0280->blendTopBg[data_027c0280->layer0Bg] = 1;
  data_027c0280->blendTopBg[data_027c0280->layer1Bg] = 0;
  data_027c0280->blendTopBg[data_027c0280->layer2Bg] = 0;
  data_027c0280->blendTopBg[data_027c0280->layer3Bg] = 0;
  data_027c0280->blendTopObj = 0;
  data_027c0280->blendBottomBackdrop = 0;
  data_027c0280->blendBottomBg[data_027c0280->layer0Bg] = 0;
  data_027c0280->blendBottomBg[data_027c0280->layer1Bg] = 0;
  data_027c0280->blendBottomBg[data_027c0280->layer2Bg] = 1;
  data_027c0280->blendBottomBg[data_027c0280->layer3Bg] = 0;
  data_027c0280->blendBottomObj = 0;
  data_027c0280->dispCnt |= 0x40;
  data_027c0280->unk_dd = 0;
  data_027c0280->bgScrollY[data_027c0280->layer0Bg] = -64;
  data_027c0280->bgScrollX[data_027c0280->layer0Bg] = 0;
  data_027c0280->bgScrollY[data_027c0280->layer1Bg] = 0;
  data_027c0280->bgScrollX[data_027c0280->layer1Bg] = 0;
  data_027c0280->bgScrollY[data_027c0280->layer2Bg] = 0;
  data_027c0280->bgScrollX[data_027c0280->layer2Bg] = 0;
  data_027c0280->bgScrollY[data_027c0280->layer3Bg] = 0;
  data_027c0280->bgScrollX[data_027c0280->layer3Bg] = 0;
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);

  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
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
  data_027c0280->bgScrollY[data_027c0280->layer2Bg] = -8;
  data_027c0280->bgScrollX[data_027c0280->layer2Bg] = 0;
  data_027c0280->bgScrollY[data_027c0280->layer3Bg] = 0;
  data_027c0280->bgScrollX[data_027c0280->layer3Bg] = 0;
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);

  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  FS_ChangeDir(data_ov005_02368a08);
  File_UnpackTo(
      data_027c0280->bgVram +
          (data_027c0280->bgCnt[data_027c0280->layer0Bg].charBase << 14),
      gRikiishiFile016);
  File_UnpackTo(data_027c0280->layer0Map, gRikiishiFile018);
  Palette_LoadFromFile(gRikiishiFile01b, 0, 0xa0, 0x60);
  map = data_027c0280->layer0Map;
  TileMap_AddToEntries(
      map,
      Screen_IsTextBg(0)
          ? data_ov005_02367f2c[GFX_LAYER(data_027c0280, 0)->screenSize]
      : Screen_IsAffineBg(0)
          ? data_ov005_02367f3c[GFX_LAYER(data_027c0280, 0)->screenSize]
          : data_ov005_02367f3c[GFX_LAYER(data_027c0280, 0)->screenSize] * 2,
      0x5000);
  Gfx_RequestLayerCopy(0);
  File_UnpackTo(
      data_027c0280->bgVram +
          (data_027c0280->bgCnt[data_027c0280->layer2Bg].charBase << 14),
      gRikiishiFile013);
  File_UnpackTo(data_027c0280->layer2Map, gRikiishiFile014);
  map = data_027c0280->layer2Map;
  TileMap_AddToEntries(
      map,
      Screen_IsTextBg(2)
          ? data_ov005_02367f2c[GFX_LAYER(data_027c0280, 2)->screenSize]
      : Screen_IsAffineBg(2)
          ? data_ov005_02367f3c[GFX_LAYER(data_027c0280, 2)->screenSize]
          : data_ov005_02367f3c[GFX_LAYER(data_027c0280, 2)->screenSize] * 2,
      0x8000);
  Palette_LoadFromFile(gRikiishiFile015, 0, 0x100, 0x100);
  Gfx_RequestLayerCopy(2);
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);

  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  FS_ChangeDir(data_ov005_02368a08);
  File_UnpackTo(
      data_027c0280->bgVram +
          (data_027c0280->bgCnt[data_027c0280->layer2Bg].charBase << 14),
      gRikiishiFile01f);
  File_UnpackTo(data_027c0280->layer2Map, gRikiishiFile020);
  map = data_027c0280->layer2Map;
  TileMap_AddToEntries(
      map,
      Screen_IsTextBg(2)
          ? data_ov005_02367f2c[GFX_LAYER(data_027c0280, 2)->screenSize]
      : Screen_IsAffineBg(2)
          ? data_ov005_02367f3c[GFX_LAYER(data_027c0280, 2)->screenSize]
          : data_ov005_02367f3c[GFX_LAYER(data_027c0280, 2)->screenSize] * 2,
      0x6000);
  Palette_LoadFromFile(gRikiishiFile021, 0, 0xc0, 0x140);
  Gfx_RequestLayerCopy(2);
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);

  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  Mem_Zero32(0, data_027c0280->unk_f8, 0x800);
  Font_LoadDigits(0);
  Palette_Load(data_021388f0, 0, 32);
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);

  work->songFadeTask = Sound_StartSongSwitch(54, 0);
  work->unk_04 = 0;
  task->step = 0;
}
