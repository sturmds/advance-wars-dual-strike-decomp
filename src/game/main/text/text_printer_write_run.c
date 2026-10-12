#include "types.h"
#include "game/text_printer.h"

u32 Text_WriteCachedString(u16* entry, const u8* text, u16 tileBase, int arg);

void TextPrinter_WriteRun(TextPrinter* printer, u16* entry, int arg) {
  u32 count;

  count = Text_WriteCachedString(entry, printer->text,
                                 printer->unk_30 + printer->unk_32, arg);
  printer->cursorX += count;
  printer->text = (const u8*)printer->text + count;
}
