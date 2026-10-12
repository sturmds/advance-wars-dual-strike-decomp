#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/Unk_0229c734.h"

void Screen_SetCurrent(void* state);
void Screen_SetCurrentGfx(void* state);
void Palette_Load(const void* src, u32 offset, u32 size);
u8* File_Load(const char* path);
void* Mem_Alloc(u32 size);

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern Unk_027c0288 data_027c0288;
extern const char data_0215a4e4[];

/* Allocates data_0229c734, loads /ohashi/res_result_point_obj and its three
 * OBJ palettes from paletteSlot, and empties the score list. Graphics parts
 * go to OBJ tiles from firstTile. */
void ResultPoint_Init(int firstTile, int paletteSlot) {
  int i;
  Unk_027c0280* savedGfx;
  Unk_02080440* saved;

  data_0229c734 = Mem_Alloc(sizeof(Unk_0229c734));
  data_0229c734->nextTile = firstTile;
  data_0229c734->paletteSlot = paletteSlot;
  data_0229c734->gfxFile = File_Load(data_0215a4e4);
  for (i = 0; i < SCORE_ROW_COUNT; i++) {
    data_0229c734->rows[i].kind = 0;
  }
  for (i = 0; i < RESULT_POINT_PART_COUNT; i++) {
    data_0229c734->partTile[i] = 0xffff;
  }
  savedGfx = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  Palette_Load(data_0229c734->gfxFile + 0x988,
               (u16)((data_0229c734->paletteSlot + 16) * 32), 96);
  Screen_SetCurrent(saved);
  Screen_SetCurrentGfx(savedGfx);
  data_0229c734->firstRow = 0;
  data_0229c734->pageRowCount = 0;
  data_0229c734->topMargin = 0;
  data_0229c734->pageIndex = 0;
  data_0229c734->pageMaxPoints = 0;
  data_0229c734->rowCount = 0;
  data_0229c734->unk_a4 = 1;
}
