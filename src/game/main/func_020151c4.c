#include "types.h"
#include "game/Unk_02088d04.h"
#include "game/script_slot.h"

extern Unk_027c0280* data_027c0280;
extern u8 gEventSkipState;
extern ScriptCommand data_021583b4[];
void TextPrinter_Init(void* printer, int x, int y, u8* tiles, u16* map,
                      const void* text, u16 unk_30, u16 unk_32);
void TextPrinter_ShowAll(void* printer);
s8 ScriptSlot_Start(const void* descriptor, int arg);

void func_020151c4(int x, int y, u16* map, const void* text, u16 unk_30,
                   u16 unk_32) {
  Unk_027c0280* gfx = data_027c0280;
  void* printer = gfx->textPrinter;

  gEventSkipState = 0;
  TextPrinter_Init(printer, x, y,
                   gfx->bgVram + (gfx->bgCnt[gfx->layer0Bg].charBase << 14),
                   map, text, unk_30, unk_32);
  TextPrinter_ShowAll(printer);
  ScriptSlot_Start(data_021583b4, 0);
}
