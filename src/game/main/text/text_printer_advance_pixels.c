#include "types.h"
#include "game/text_printer.h"

/* Moves the printer right by pixels. pixelInTile keeps the pixel offset inside the
 * current tile; each full 8 pixels moves cursorX on one tile. Returns TRUE
 * when the cursor ends part way into a tile. */
BOOL TextPrinter_AdvancePixels(TextPrinter* printer, int pixels) {
  BOOL partial = FALSE;

  printer->pixelInTile += pixels;
  while (printer->pixelInTile >= 8) {
    printer->pixelInTile -= 8;
    printer->unk_38 += 2;
    printer->cursorX++;
  }
  if (printer->pixelInTile != 0) {
    partial = TRUE;
  }
  return partial;
}
