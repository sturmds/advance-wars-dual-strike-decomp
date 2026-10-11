#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/ov005_task.h"
#include "game/text_printer.h"
#include "game/task.h"
#include "game/Unk_ov005_0236bb80.h"

void Screen_SetCurrent(void *state);
void Screen_SetCurrentGfx(void *state);
void Screen_SetCurrentBoth(void *state);
void Screen_SetBgMode(int mode);
void Screen_InitLayers(const void *config);
void Font_LoadDigits(int baseTile);
void Palette_Load(const void *src, u32 offset, u32 size);
void FS_ChangeDir(const char *dir);
void File_UnpackTo(void *dest, const char *fileName);
void Palette_LoadFromFile(const char *fileName, int fileOffset, int paletteOffset, int size);
void Gfx_RequestLayerCopy(int layer);
TextPrinter *Text_PrintByIdOnLayer0(int x, int y, u16 *map, u32 textId, u16 unk_30, u16 unk_38);
Task *Sound_StartSongSwitch(int song, int arg); /* task: fade out the song, then start `song` */

extern Unk_02080440 *data_027c027c;
extern Unk_027c0280 *data_027c0280;
extern Unk_027c0288 data_027c0288;
extern u8 data_ov005_023680f0[];       /* main screen layer config */
extern u8 gOv005DarkenedSubLayers[];       /* sub screen layer config */
extern u8 data_021388f0[];
extern char data_ov005_02368288[];     /* "/rikiishi" */
extern char gRikiishiFile000[];
extern char gRikiishiFile001[];
extern char gRikiishiFile002[];
extern char gRikiishiFile003[];
extern char gRikiishiFile004[];

/* Screen setup step of an overlay 5 screen. Main screen: BG0 and OBJ on,
 * the digit font, and a text printer on layer 0. Sub screen: BG1, BG2 and OBJ
 * on, both layers and the palette from /rikiishi files 000 to 004, BG2
 * scrolled down 256 pixels. Starts song 51. Both screens end fully darkened
 * (darken mode, evy 16), so a later step can fade them in. */
void Ov005_SetupDarkenedScreensWithSong(Ov005Task *task)
{
    Unk_ov005_0236bb80 *work;
    Unk_027c0280 *saved0280;
    Unk_02080440 *saved027c;
    TextPrinter *printer;

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
    Screen_InitLayers(gOv005DarkenedSubLayers);
    data_027c0280->dispCnt &= ~0x100;
    data_027c0280->dispCnt |= 0x200;
    data_027c0280->dispCnt |= 0x400;
    data_027c0280->dispCnt &= ~0x800;
    data_027c0280->dispCnt |= 0x1000;
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);

    saved0280 = data_027c0280;
    saved027c = data_027c027c;
    Screen_SetCurrentBoth(data_027c0288.unk_00);
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
    Screen_SetCurrentBoth(data_027c0288.unk_04);
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
    Screen_SetCurrentGfx(data_027c0288.unk_00);
    saved027c = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_00);
    Font_LoadDigits(0);
    Palette_Load(data_021388f0, 0, 32);
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);

    saved0280 = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_00);
    saved027c = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_00);
    printer = Text_PrintByIdOnLayer0(6, 6, data_027c0280->layer0Map, 0x21000000, 0, 0x41);
    work->textPrinter = printer;
    printer->unk_3e = 6;
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);

    saved0280 = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_04);
    saved027c = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_04);
    FS_ChangeDir(data_ov005_02368288);
    File_UnpackTo(data_027c0280->bgVram
                      + (data_027c0280->bgCnt[data_027c0280->layer1Bg].charBase << 14),
                  gRikiishiFile000);
    File_UnpackTo(data_027c0280->layer1Map, gRikiishiFile001);
    File_UnpackTo(data_027c0280->bgVram
                      + (data_027c0280->bgCnt[data_027c0280->layer2Bg].charBase << 14),
                  gRikiishiFile002);
    File_UnpackTo(data_027c0280->layer2Map, gRikiishiFile003);
    Palette_LoadFromFile(gRikiishiFile004, 0, 0, 0x200);
    Gfx_RequestLayerCopy(1);
    Gfx_RequestLayerCopy(2);
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);

    task->step = 0;
    work->unk_00 = 0;
    work->unk_04 = 500;

    saved0280 = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_04);
    saved027c = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_04);
    data_027c0280->bgScrollY[data_027c0280->layer1Bg] = 0;
    data_027c0280->bgScrollY[data_027c0280->layer2Bg] = 256;
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);

    work->songTask = Sound_StartSongSwitch(51, 1);

    saved0280 = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_00);
    saved027c = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_00);
    data_027c0280->blendMode = 0;
    data_027c0280->blendEva = 0;
    data_027c0280->blendEvb = 0;
    data_027c0280->blendEvy = 0;
    data_027c0280->blendTopBg[data_027c0280->layer0Bg] = 1;
    data_027c0280->blendTopBg[data_027c0280->layer1Bg] = 1;
    data_027c0280->blendTopBg[data_027c0280->layer2Bg] = 1;
    data_027c0280->blendTopBg[data_027c0280->layer3Bg] = 1;
    data_027c0280->blendTopObj = 1;
    data_027c0280->blendTopBackdrop = 1;
    data_027c0280->blendBottomBg[data_027c0280->layer0Bg] = 0;
    data_027c0280->blendBottomBg[data_027c0280->layer1Bg] = 0;
    data_027c0280->blendBottomBg[data_027c0280->layer2Bg] = 0;
    data_027c0280->blendBottomBg[data_027c0280->layer3Bg] = 0;
    data_027c0280->blendBottomObj = 0;
    data_027c0280->blendBottomBackdrop = 0;
    data_027c0280->blendMode = BLEND_MODE_DARKEN;
    data_027c0280->blendEvy = 16;
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);

    saved0280 = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_04);
    saved027c = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_04);
    data_027c0280->blendMode = 0;
    data_027c0280->blendEva = 0;
    data_027c0280->blendEvb = 0;
    data_027c0280->blendEvy = 0;
    data_027c0280->blendTopBg[data_027c0280->layer0Bg] = 1;
    data_027c0280->blendTopBg[data_027c0280->layer1Bg] = 1;
    data_027c0280->blendTopBg[data_027c0280->layer2Bg] = 1;
    data_027c0280->blendTopBg[data_027c0280->layer3Bg] = 1;
    data_027c0280->blendTopObj = 1;
    data_027c0280->blendTopBackdrop = 1;
    data_027c0280->blendBottomBg[data_027c0280->layer0Bg] = 0;
    data_027c0280->blendBottomBg[data_027c0280->layer1Bg] = 0;
    data_027c0280->blendBottomBg[data_027c0280->layer2Bg] = 0;
    data_027c0280->blendBottomBg[data_027c0280->layer3Bg] = 0;
    data_027c0280->blendBottomObj = 0;
    data_027c0280->blendBottomBackdrop = 0;
    data_027c0280->blendMode = BLEND_MODE_DARKEN;
    data_027c0280->blendEvy = 16;
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);
}
