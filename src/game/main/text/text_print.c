#include "types.h"
#include "game/Unk_02088d04.h"
#include "game/text_print.h"
#include "game/script_slot.h"

extern u8 gEventSkipState;
extern ScriptCommand data_02158394[];

/* sets up printer to print text at tile (x, y) into map, with tile data at tiles */
void TextPrinter_Init(void* printer, int x, int y, u8* tiles, u16* map,
                      const void* text, u16 unk_30, u16 unk_32);
void TextPrinter_ShowAll(void* printer);
s8 ScriptSlot_Start(const void* descriptor,
                    int arg); /* starts a task from descriptor */

/* Prints text at tile (x, y) of map, drawing the glyphs into the layer 0
 * tile data, through the printer in data_027c0280 and the task that
 * data_02158394 describes; returns that task's script slot. */
s8 Text_Print(s16 x, s16 y, u16* map, const void* text, u16 unk_30,
              u16 unk_32) {
  Unk_027c0280* gfx = data_027c0280;
  void* printer = gfx->textPrinter;

  gEventSkipState = 0;
  TextPrinter_Init(printer, x, y,
                   gfx->bgVram + (gfx->bgCnt[gfx->layer0Bg].charBase << 14),
                   map, text, unk_30, unk_32);
  TextPrinter_ShowAll(printer);
  return ScriptSlot_Start(data_02158394, 0);
}
