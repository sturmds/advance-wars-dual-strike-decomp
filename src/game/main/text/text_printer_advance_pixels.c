#include "types.h"
#include "game/text_printer.h"

/* Moves the printer right by pixels. unk_44 keeps the pixel offset inside the
 * current tile; each full 8 pixels moves cursorX on one tile. Returns TRUE
 * when the cursor ends part way into a tile. */
BOOL TextPrinter_AdvancePixels(TextPrinter *printer, int pixels)
{
    BOOL partial = FALSE;

    printer->unk_44 += pixels;
    while (printer->unk_44 >= 8) {
        printer->unk_44 -= 8;
        printer->unk_38 += 2;
        printer->cursorX++;
    }
    if (printer->unk_44 != 0) {
        partial = TRUE;
    }
    return partial;
}
