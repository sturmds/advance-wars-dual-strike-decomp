#include "types.h"
#include "game/text_printer.h"

const void* Text_Get(u32 textId);
void TextPrinter_Init(TextPrinter* printer, int x, int y, u8* tiles, u16* map,
                      const void* text, u16 unk_30, u16 unk_38);

/* TextPrinter_Init for a text id instead of a string. */
void TextPrinter_InitById(TextPrinter* printer, int x, int y, u8* tiles,
                          u16* map, u32 textId, u16 unk_30, u16 unk_38) {
  TextPrinter_Init(printer, x, y, tiles, map, Text_Get(textId), unk_30, unk_38);
}
