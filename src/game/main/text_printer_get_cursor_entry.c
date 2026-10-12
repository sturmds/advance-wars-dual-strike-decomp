#include "types.h"
#include "game/text_printer.h"

u16* TextPrinter_GetCursorEntry(TextPrinter* printer) {
  return printer->screen + printer->cursorX + printer->cursorY * 32;
}
