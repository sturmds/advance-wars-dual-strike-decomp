#include "types.h"
#include "game/task.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/ov005_task.h"
#include "game/Unk_ov005_0236bb8c.h"
#include "game/screen_other_map.h"
#include "game/screen.h"
#include "sdk/fs.h"

void File_UnpackTo(void* dest, const char* fileName);
void Palette_LoadFromFile(const char* fileName, int fileOffset,
                          int paletteOffset, int size);
void Palette_Load(const void* src, u32 offset, u32 size);
void TileMap_AddToEntries(u16* entries, u16 byteCount, int addend);
BOOL Screen_IsTextBg(int layer);
void Gfx_RequestLayerCopy(int layer);
void Font_LoadDigits(int baseTile);
Task* Sound_StartSongSwitch(
    int song, int arg); /* task: fade out the song, then start `song` */

extern u8 data_021388f0[];
extern GfxLayer gOv005MessageMainLayers[]; /* main screen layer config */
extern GfxLayer gOv005MessageSubLayers[];  /* sub screen layer config */
extern char data_ov005_02369594[];         /* "/rikiishi" */
extern char gOv005MessageFile016[];
extern char gOv005MessageFile017[];
extern char gOv005MessageFile01b[];
extern char gOv005MessageFile013[];
extern char gOv005MessageFile014[];
extern char gOv005MessageFile015[];
extern char gOv005MessageFile01c[];
extern char gOv005MessageFile01d[];
extern char gOv005MessageFile01e[];
/* map bytes of a rotation/scaling BG by screen size */
static const u32 data_ov005_02367f6c[] = {
#include "data/ov005/data_ov005_02367f6c.inc.c"
};
/* map bytes of a text BG by screen size */
static const u32 data_ov005_02367f7c[] = {
#include "data/ov005/data_ov005_02367f7c.inc.c"
};

/* First step of the message screen (Ov005_UpdateMessageScreen runs next).
 * Sub screen: BG0 and BG2 on, BG0 alpha-blended over BG2 at eva 0 (the update
 * fades it in), BG0 scrolled 64 pixels down; both layers come from /rikiishi
 * files 013 to 01b. Main screen: BG0, BG2 and OBJ on, window 0 spans
 * lines 24 to 171 with colour effects on inside, alpha blend at eva 8 and
 * evb 8 with BG2 and the backdrop as first targets; BG2 comes from
 * /rikiishi files 01c to 01e. Loads the digit font, then
 * starts a task that fades to song 0x35. */
void Ov005_SetUpMessageScreen(Ov005Task* task) {
  Unk_ov005_0236bb8c* work = task->work;
  Unk_027c0280* savedGfx;
  Unk_02080440* savedScreen;
  u16* map;

  savedGfx = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  savedScreen = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  Screen_SetBgMode(0);
  Screen_InitLayers(gOv005MessageMainLayers);
  data_027c0280->dispCnt |= 0x100;
  data_027c0280->dispCnt &= ~0x200;
  data_027c0280->dispCnt |= 0x400;
  data_027c0280->dispCnt &= ~0x800;
  data_027c0280->dispCnt |= 0x1000;
  Screen_SetCurrent(savedScreen);
  Screen_SetCurrentGfx(savedGfx);

  savedGfx = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  savedScreen = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  Screen_SetBgMode(0);
  Screen_InitLayers(gOv005MessageSubLayers);
  data_027c0280->dispCnt |= 0x100;
  data_027c0280->dispCnt &= ~0x200;
  data_027c0280->dispCnt |= 0x400;
  data_027c0280->dispCnt &= ~0x800;
  data_027c0280->dispCnt &= ~0x1000;
  Screen_SetCurrent(savedScreen);
  Screen_SetCurrentGfx(savedGfx);

  savedGfx = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  savedScreen = data_027c027c;
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
  Screen_SetCurrent(savedScreen);
  Screen_SetCurrentGfx(savedGfx);

  savedGfx = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  savedScreen = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  data_027c0280->bgCnt[data_027c0280->layer0Bg].priority = 0;
  data_027c0280->bgCnt[data_027c0280->layer1Bg].priority = 1;
  data_027c0280->bgCnt[data_027c0280->layer2Bg].priority = 2;
  data_027c0280->bgCnt[data_027c0280->layer3Bg].priority = 3;
  data_027c0280->dispCnt |= 0x2000;
  data_027c0280->dispCnt &= ~0x4000;
  data_027c0280->dispCnt &= ~0x8000;
  data_027c0280->win0In.effect = 1;
  data_027c0280->win0In.bg[data_027c0280->layer0Bg] = 1;
  data_027c0280->win0In.bg[data_027c0280->layer1Bg] = 0;
  data_027c0280->win0In.bg[data_027c0280->layer2Bg] = 1;
  data_027c0280->win0In.bg[data_027c0280->layer3Bg] = 0;
  data_027c0280->win0In.obj = 1;
  data_027c0280->winOut.effect = 0;
  data_027c0280->winOut.bg[data_027c0280->layer0Bg] = 1;
  data_027c0280->winOut.bg[data_027c0280->layer1Bg] = 0;
  data_027c0280->winOut.bg[data_027c0280->layer2Bg] = 1;
  data_027c0280->winOut.bg[data_027c0280->layer3Bg] = 0;
  data_027c0280->winOut.obj = 1;
  data_027c0280->win0Left = 0;
  data_027c0280->win0Top = 24;
  data_027c0280->win0Right = 255;
  data_027c0280->win0Bottom = 171;
  data_027c0280->blendMode = BLEND_MODE_ALPHA;
  data_027c0280->blendEva = 8;
  data_027c0280->blendEvb = 8;
  data_027c0280->blendEvy = 0;
  data_027c0280->blendTopBackdrop = 1;
  data_027c0280->blendTopBg[data_027c0280->layer0Bg] = 0;
  data_027c0280->blendTopBg[data_027c0280->layer1Bg] = 0;
  data_027c0280->blendTopBg[data_027c0280->layer2Bg] = 1;
  data_027c0280->blendTopBg[data_027c0280->layer3Bg] = 0;
  data_027c0280->blendTopObj = 0;
  data_027c0280->blendBottomBackdrop = 1;
  data_027c0280->blendBottomBg[data_027c0280->layer0Bg] = 0;
  data_027c0280->blendBottomBg[data_027c0280->layer1Bg] = 0;
  data_027c0280->blendBottomBg[data_027c0280->layer2Bg] = 0;
  data_027c0280->blendBottomBg[data_027c0280->layer3Bg] = 0;
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
  FS_ChangeDir(data_ov005_02369594);
  File_UnpackTo(
      data_027c0280->bgVram +
          (data_027c0280->bgCnt[data_027c0280->layer0Bg].charBase << 14),
      gOv005MessageFile016);
  File_UnpackTo(data_027c0280->layer0Map, gOv005MessageFile017);
  Palette_LoadFromFile(gOv005MessageFile01b, 0, 0xa0, 0x60);
  map = data_027c0280->layer0Map;
  TileMap_AddToEntries(
      map,
      Screen_IsTextBg(0)
          ? data_ov005_02367f7c[GFX_LAYER(data_027c0280, 0)->screenSize]
      : Screen_IsAffineBg(0)
          ? data_ov005_02367f6c[GFX_LAYER(data_027c0280, 0)->screenSize]
          : data_ov005_02367f6c[GFX_LAYER(data_027c0280, 0)->screenSize] * 2,
      0x5000);
  Gfx_RequestLayerCopy(0);
  File_UnpackTo(
      data_027c0280->bgVram +
          (data_027c0280->bgCnt[data_027c0280->layer2Bg].charBase << 14),
      gOv005MessageFile013);
  File_UnpackTo(data_027c0280->layer2Map, gOv005MessageFile014);
  map = data_027c0280->layer2Map;
  TileMap_AddToEntries(
      map,
      Screen_IsTextBg(2)
          ? data_ov005_02367f7c[GFX_LAYER(data_027c0280, 2)->screenSize]
      : Screen_IsAffineBg(2)
          ? data_ov005_02367f6c[GFX_LAYER(data_027c0280, 2)->screenSize]
          : data_ov005_02367f6c[GFX_LAYER(data_027c0280, 2)->screenSize] * 2,
      0x8000);
  Palette_LoadFromFile(gOv005MessageFile015, 0, 0x100, 0x100);
  Gfx_RequestLayerCopy(2);
  Screen_SetCurrent(savedScreen);
  Screen_SetCurrentGfx(savedGfx);

  savedGfx = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  savedScreen = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  FS_ChangeDir(data_ov005_02369594);
  File_UnpackTo(
      data_027c0280->bgVram +
          (data_027c0280->bgCnt[data_027c0280->layer2Bg].charBase << 14),
      gOv005MessageFile01c);
  File_UnpackTo(data_027c0280->layer2Map, gOv005MessageFile01d);
  map = data_027c0280->layer2Map;
  TileMap_AddToEntries(
      map,
      Screen_IsTextBg(2)
          ? data_ov005_02367f7c[GFX_LAYER(data_027c0280, 2)->screenSize]
      : Screen_IsAffineBg(2)
          ? data_ov005_02367f6c[GFX_LAYER(data_027c0280, 2)->screenSize]
          : data_ov005_02367f6c[GFX_LAYER(data_027c0280, 2)->screenSize] * 2,
      0x8000);
  Palette_LoadFromFile(gOv005MessageFile01e, 0, 0x100, 0x100);
  Gfx_RequestLayerCopy(2);
  Screen_SetCurrent(savedScreen);
  Screen_SetCurrentGfx(savedGfx);

  savedGfx = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  savedScreen = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  Font_LoadDigits(0);
  Palette_Load(data_021388f0, 0, 32);
  Screen_SetCurrent(savedScreen);
  Screen_SetCurrentGfx(savedGfx);

  work->songTask = Sound_StartSongSwitch(0x35, 0);
  task->step = 0;
}
