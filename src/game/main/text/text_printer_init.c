#include "types.h"
#include "game/text_printer.h"

void TextPrinter_InitFull(TextPrinter* printer, int x, int y, u8* tiles,
                          u16* map, const void* text, u16 unk_30, u16 unk_32,
                          u16 unk_38);

/* Sets up printer to write text at tile (x, y) of map, with the glyph tile
 * data at tiles. Same as TextPrinter_InitFull with the 0x32 field set to 0. */
void TextPrinter_Init(TextPrinter* printer, int x, int y, u8* tiles, u16* map,
                      const void* text, u16 unk_30, u16 unk_38) {
  TextPrinter_InitFull(printer, x, y, tiles, map, text, unk_30, 0, unk_38);
}
