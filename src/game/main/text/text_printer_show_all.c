#include "types.h"
#include "game/text_printer.h"

void TextPrinter_ShowAll(TextPrinter* printer) {
  printer->charDelay = 0;
  printer->frameCounter = -2;
}
