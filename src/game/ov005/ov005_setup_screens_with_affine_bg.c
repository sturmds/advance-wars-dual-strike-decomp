#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/affine_params.h"
#include "game/ov005_task.h"
#include "game/text_printer.h"
#include "game/Unk_ov005_0236bb80.h"

void Screen_SetCurrent(void* state);
void Screen_SetCurrentGfx(void* state);
void Screen_SetCurrentBoth(void* state);
void Screen_SetBgMode(int mode);
void Screen_InitLayers(const void* config);
void FS_ChangeDir(const char* dir);
void File_UnpackTo(void* dest, const char* fileName);
void* File_Load(const char* path);
void Palette_LoadFromFile(const char* fileName, int fileOffset,
                          int paletteOffset, int size);
void BgExtPalette_LoadFromFile(const char* fileName, int a, int b, int c);
void Gfx_RequestLayerCopy(int layer);
TextPrinter* Text_PrintByIdOnLayer0(int x, int y, u16* map, u32 textId,
                                    u16 unk_30, u16 unk_38);

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern Unk_027c0288 data_027c0288;
extern GfxLayer data_ov005_023680f0[];   /* main screen layer config */
extern GfxLayer gOv005AffineSubLayers[]; /* sub screen layer config */
extern char data_ov005_02368288[];       /* "/rikiishi" */
extern char gRikiishiFile00d[];
extern char gRikiishiFile00e[];
extern char gRikiishiFile00f[];
extern char gRikiishiFile010[];
extern char gRikiishiFile012[];
extern char gRikiishiFile011[];

/* Screen setup step of an overlay 5 screen (task script data_ov005_02368230).
 * Main screen: mode 0, BG0 and OBJ on. Sub screen: mode 5, OBJ on, the BG2
 * rotation/scaling matrix reset to identity. Both: layer priorities 0 to 3,
 * no windows or blending, no scroll. Then loads the sub screen's layer 2
 * and object graphics from /rikiishi, keeps file 011 in the work block, and
 * starts a text printer on the main screen's layer 0. */
void Ov005_SetupScreensWithAffineBg(Ov005Task* task) {
  Unk_ov005_0236bb80* work;
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;
  AffineParams params;
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
  Screen_SetBgMode(5);
  Screen_InitLayers(gOv005AffineSubLayers);
  data_027c0280->dispCnt &= ~0x100;
  data_027c0280->dispCnt &= ~0x200;
  data_027c0280->dispCnt &= ~0x400;
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
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  params.srcX = 0;
  params.srcY = 0;
  params.angle = 0;
  params.centerX = 0;
  params.centerY = 0;
  params.scaleX = 256;
  params.scaleY = 256;
  BgAffine_FromParams(&params, &data_027c0280->bg2Affine);
  Screen_SetCurrentGfx(saved0280);

  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  FS_ChangeDir(data_ov005_02368288);
  File_UnpackTo(
      data_027c0280->bgVram +
          (data_027c0280->bgCnt[data_027c0280->layer2Bg].charBase << 14),
      gRikiishiFile00d);
  File_UnpackTo(data_027c0280->layer2Map, gRikiishiFile00e);
  BgExtPalette_LoadFromFile(gRikiishiFile00f, 0, 0x200, 2);
  Gfx_RequestLayerCopy(2);
  File_UnpackTo(data_027c0280->objVram, gRikiishiFile010);
  Palette_LoadFromFile(gRikiishiFile012, 0, 0x200, 0x80);
  work->unk_b4 = File_Load(gRikiishiFile011);
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);

  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  printer = Text_PrintByIdOnLayer0(6, 2, data_027c0280->layer0Map, 0x21000002,
                                   0, 0x41);
  work->textPrinter = printer;
  printer->charDelay = 6;
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);

  task->step = 0;
}
