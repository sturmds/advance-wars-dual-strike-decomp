#include "types.h"
#include "game/task.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/Unk_022a7a9c.h"

void Screen_SetCurrent(void* state);
void Screen_SetCurrentGfx(void* state);
void Screen_SetCurrentBoth(void* state);
void Gfx_LoadBmap09f(void* dst, int unused, int paletteSlot);
void Font_LoadDigits(int baseTile);
void Gfx_ClearLayerMap(int layer);
void Co_LoadArmyGraphicsAndPalette(int coId, int firstTile, int paletteSlot,
                                   int army);
void func_0201e6e8(int x, int y, int width, int height);
void Co_UnpackPicture(int coId, int firstTile);
void Palette_LoadObjRow(int row);

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern Unk_027c0288 data_027c0288;

#define RECORD_INDEX(task) ((task)->unk_64)

/* Task step: copies data_022a7a9c.player into task->unk_64 (and that minus 1
 * into unk_66), gives both screens layer priorities 0 to 3 and no blending,
 * then loads the digit font and the picture and palette of the CO in record
 * unk_64 of data_022a7ab8, plus a second CO when the record has one.
 * Layer 1 gets scroll Y -168 and screen size 2. */

void WinQuote_Init(Task* task) {
  Unk_027c0280* savedGfx;
  Unk_02080440* saved;

  task->unk_64 = data_022a7a9c.player;
  task->unk_66 = RECORD_INDEX(task) - 1;
  Gfx_LoadBmap09f(data_027c0280->unk_f8, 0, 0);

  savedGfx = data_027c0280;
  saved = data_027c027c;
  Screen_SetCurrentBoth(data_027c0288.unk_00);
  data_027c0280->bgCnt[data_027c0280->layer0Bg].priority = 0;
  data_027c0280->bgCnt[data_027c0280->layer1Bg].priority = 1;
  data_027c0280->bgCnt[data_027c0280->layer2Bg].priority = 2;
  data_027c0280->bgCnt[data_027c0280->layer3Bg].priority = 3;
  data_027c0280->blendMode = 0;
  data_027c0280->blendEva = 16;
  data_027c0280->blendEvb = 0;
  data_027c0280->blendEvy = 0;
  Screen_SetCurrentBoth(data_027c0288.unk_04);
  data_027c0280->bgCnt[data_027c0280->layer0Bg].priority = 0;
  data_027c0280->bgCnt[data_027c0280->layer1Bg].priority = 1;
  data_027c0280->bgCnt[data_027c0280->layer2Bg].priority = 2;
  data_027c0280->bgCnt[data_027c0280->layer3Bg].priority = 3;
  data_027c0280->blendMode = 0;
  data_027c0280->blendEva = 16;
  data_027c0280->blendEvb = 0;
  data_027c0280->blendEvy = 0;
  Screen_SetCurrent(saved);
  Screen_SetCurrentGfx(savedGfx);

  Font_LoadDigits(0);
  Gfx_ClearLayerMap(1);
  /* needed to match: the original has the record index in r3 (the army
     * argument) at both calls */
  Co_LoadArmyGraphicsAndPalette(data_022a7ab8[RECORD_INDEX(task)].unk_00, 0x224,
                                5, RECORD_INDEX(task));
  func_0201e6e8(0, 17, 32, 4);
  if (data_022a7aba[RECORD_INDEX(task)].unk_00 & 0x7f) {
    Co_LoadArmyGraphicsAndPalette(data_022a7aba[RECORD_INDEX(task)].unk_00,
                                  0x3d4, 6, RECORD_INDEX(task));
    func_0201e6e8(0, 12, 32, 4);
  }
  Co_UnpackPicture(data_022a7ab8[RECORD_INDEX(task)].unk_00 & 0x7f, 0x1dc);
  if (data_022a7aba[RECORD_INDEX(task)].unk_00 & 0x7f) {
    Co_UnpackPicture(data_022a7aba[RECORD_INDEX(task)].unk_00 & 0x7f, 0x1e8);
  }
  Palette_LoadObjRow(4);
  data_027c0280->bgScrollX[data_027c0280->layer1Bg] = 0;
  data_027c0280->bgScrollY[data_027c0280->layer1Bg] = -168;
  data_027c0280->bgCnt[data_027c0280->layer1Bg].screenSize = 2;
}
