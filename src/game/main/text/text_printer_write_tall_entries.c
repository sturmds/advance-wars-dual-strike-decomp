#include "types.h"
#include "game/text_printer.h"

/* Writes the two map entries for a 1x2 tile glyph: tile unk_38 on the top row
 * and the next tile one row down (32 entries on), each with unk_30 ORed in. */
void TextPrinter_WriteTallEntries(TextPrinter* printer, u16* entry) {
  entry[0] = printer->unk_38 | printer->unk_30;
  entry[32] = (printer->unk_38 + 1) | printer->unk_30;
}
