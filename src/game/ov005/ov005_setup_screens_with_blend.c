#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/ov005_task.h"
#include "game/text_printer.h"
#include "game/Unk_ov005_0236bb80.h"
#include "game/screen_other_map.h"
#include "game/screen.h"
#include "sdk/fs.h"

void File_UnpackTo(void* dest, const char* fileName);
void Palette_LoadFromFile(const char* fileName, int fileOffset,
                          int paletteOffset, int size);
void TileMap_AddToEntries(u16* entries, u16 byteCount, int addend);
BOOL Screen_IsTextBg(int layer);
void Gfx_RequestLayerCopy(int layer);
TextPrinter* Text_PrintByIdOnLayer0(int x, int y, u16* map, u32 textId,
                                    u16 unk_30, u16 unk_38);

extern GfxLayer data_ov005_023680f0[];  /* main screen layer config */
extern GfxLayer gOv005BlendSubLayers[]; /* sub screen layer config */
extern char data_ov005_02368288[];      /* "/rikiishi" */
extern char gRikiishiFile005[];
extern char gRikiishiFile006[];
extern char gRikiishiFile007[];
extern char gRikiishiFile008[];
extern char gRikiishiFile009[];
extern char gRikiishiFile00a[];
extern char gRikiishiFile00b[];
extern char gRikiishiFile00c[];
/* map bytes of a text BG by screen size */
static const u32 data_ov005_02367f0c[] = {
#include "data/ov005/data_ov005_02367f0c.inc.c"
};
/* map bytes of a rotation/scaling BG by screen size */
static const u32 data_ov005_02367f1c[] = {
#include "data/ov005/data_ov005_02367f1c.inc.c"
};

/* Screen setup step of an overlay 5 screen (task script data_ov005_02368230).
 * Main screen: BG0 and OBJ on, no blending, a text printer on layer 0. Sub
 * screen: BG0-BG2 and OBJ on, BG0 scrolled 8 pixels and alpha-blended over
 * BG1 and BG2; the three layers come from /rikiishi files 005 to 00c. The
 * blend starts at eva 0, evb 16 (BG0 not visible) and is kept in the work
 * block as fx32, so a later step can fade BG0 in. */
void Ov005_SetupScreensWithBlend(Ov005Task* task) {
  Unk_ov005_0236bb80* work;
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;
  TextPrinter* printer;

  work = task->work;
  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  Screen_SetBgMode(0);
  Screen_InitLayers(data_ov005_023680f0);
  data_027c0280->dispCnt |= 0x100;
  data_027c0280->dispCnt &= ~0x200;
  data_027c0280->dispCnt &= ~0x400;
  data_027c0280->dispCnt &= ~0x800;
  data_027c0280->dispCnt |= 0x1000;
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);

  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  Screen_SetBgMode(0);
  Screen_InitLayers(gOv005BlendSubLayers);
  data_027c0280->dispCnt |= 0x100;
  data_027c0280->dispCnt |= 0x200;
  data_027c0280->dispCnt |= 0x400;
  data_027c0280->dispCnt &= ~0x800;
  data_027c0280->dispCnt |= 0x1000;
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
  data_027c0280->bgScrollY[data_027c0280->layer2Bg] = 0;
  data_027c0280->bgScrollX[data_027c0280->layer2Bg] = 0;
  data_027c0280->bgScrollY[data_027c0280->layer3Bg] = 0;
  data_027c0280->bgScrollX[data_027c0280->layer3Bg] = 0;
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
  data_027c0280->blendEvb = 0;
  data_027c0280->blendEvy = 0;
  data_027c0280->blendTopBg[data_027c0280->layer0Bg] = 1;
  data_027c0280->blendTopBg[data_027c0280->layer1Bg] = 0;
  data_027c0280->blendTopBg[data_027c0280->layer2Bg] = 0;
  data_027c0280->blendTopBg[data_027c0280->layer3Bg] = 0;
  data_027c0280->blendTopObj = 0;
  data_027c0280->blendBottomBg[data_027c0280->layer0Bg] = 0;
  data_027c0280->blendBottomBg[data_027c0280->layer1Bg] = 1;
  data_027c0280->blendBottomBg[data_027c0280->layer2Bg] = 1;
  data_027c0280->blendBottomBg[data_027c0280->layer3Bg] = 0;
  data_027c0280->blendBottomObj = 0;
  data_027c0280->dispCnt |= 0x40;
  data_027c0280->unk_dd = 0;
  data_027c0280->bgScrollY[data_027c0280->layer0Bg] = 0;
  data_027c0280->bgScrollX[data_027c0280->layer0Bg] = 8;
  data_027c0280->bgScrollY[data_027c0280->layer1Bg] = 0;
  data_027c0280->bgScrollX[data_027c0280->layer1Bg] = 0;
  data_027c0280->bgScrollY[data_027c0280->layer2Bg] = 0;
  data_027c0280->bgScrollX[data_027c0280->layer2Bg] = 0;
  data_027c0280->bgScrollY[data_027c0280->layer3Bg] = 0;
  data_027c0280->bgScrollX[data_027c0280->layer3Bg] = 0;
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);

  /* needed to match: the file block gets its own locals */
  {
    Unk_027c0280* saved0280 = data_027c0280;
    Unk_02080440* saved027c;
    u16* map;

    Screen_SetCurrentGfx(data_027c0288.unk_04);
    saved027c = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_04);
    FS_ChangeDir(data_ov005_02368288);
    File_UnpackTo(
        data_027c0280->bgVram +
            (data_027c0280->bgCnt[data_027c0280->layer1Bg].charBase << 14),
        gRikiishiFile005);
    File_UnpackTo(data_027c0280->layer1Map, gRikiishiFile006);
    File_UnpackTo(
        data_027c0280->bgVram +
            (data_027c0280->bgCnt[data_027c0280->layer2Bg].charBase << 14),
        gRikiishiFile007);
    File_UnpackTo(data_027c0280->layer2Map, gRikiishiFile008);
    Palette_LoadFromFile(gRikiishiFile009, 0, 0, 0x1c0);
    File_UnpackTo(
        data_027c0280->bgVram +
            (data_027c0280->bgCnt[data_027c0280->layer0Bg].charBase << 14),
        gRikiishiFile00a);
    File_UnpackTo(data_027c0280->layer0Map, gRikiishiFile00b);
    Palette_LoadFromFile(gRikiishiFile00c, 0, 0x1e0, 32);
    map = data_027c0280->layer0Map;
    TileMap_AddToEntries(
        map,
        Screen_IsTextBg(0)
            ? data_ov005_02367f0c[GFX_LAYER(data_027c0280, 0)->screenSize]
        : Screen_IsAffineBg(0)
            ? data_ov005_02367f1c[GFX_LAYER(data_027c0280, 0)->screenSize]
            : data_ov005_02367f1c[GFX_LAYER(data_027c0280, 0)->screenSize] * 2,
        0xf000);
    Gfx_RequestLayerCopy(0);
    Gfx_RequestLayerCopy(1);
    Gfx_RequestLayerCopy(2);
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);
  }

  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  printer = Text_PrintByIdOnLayer0(6, 5, data_027c0280->layer0Map, 0x21000001,
                                   0, 0x41);
  work->textPrinter = printer;
  printer->charDelay = 6;
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);

  task->step = 0;
  work->unk_00 = 0;
  work->unk_04 = 500;
  work->blendEva = 0;
  work->blendEvb = 16 * FX32_ONE;

  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  data_027c0280->blendMode = BLEND_MODE_ALPHA;
  data_027c0280->blendEva = work->blendEva >> 12;
  data_027c0280->blendEvb = work->blendEvb >> 12;
  data_027c0280->blendEvy = 0;
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
}
