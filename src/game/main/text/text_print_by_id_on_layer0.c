#include "types.h"
#include "game/Unk_02088d04.h"
#include "game/text_printer.h"

extern Unk_027c0280* data_027c0280;

TextPrinter* TextPrinter_StartById(int x, int y, u8* tiles, u16* map,
                                   u32 textId, u16 unk_30, u16 unk_38);

/* Starts a text printer for a text id, drawing glyphs into the layer 0 tile data,
 * and returns it. */
TextPrinter* Text_PrintByIdOnLayer0(int x, int y, u16* map, u32 textId,
                                    u16 unk_30, u16 unk_38) {
  Unk_027c0280* gfx = data_027c0280;

  return TextPrinter_StartById(
      x, y, gfx->bgVram + (gfx->bgCnt[gfx->layer0Bg].charBase << 14), map,
      textId, unk_30, unk_38);
}
