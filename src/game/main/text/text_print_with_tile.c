#include "types.h"
#include "game/Unk_02088d04.h"
#include "game/text_printer.h"

extern Unk_027c0280 *data_027c0280;
extern u8 gEventSkipState;
extern u8 data_02158394[];

void TextPrinter_InitFull(TextPrinter *printer, int x, int y, u8 *tiles, u16 *map, const void *text, u16 unk_30, u16 unk_32, u16 tile);
void func_02015da8(TextPrinter *printer);
s8 ScriptSlot_Start(const void *descriptor, int arg);

/* Like Text_Print, with one more argument for the printer (the last argument of
 * TextPrinter_InitFull; MapSelect_DrawMapNames passes the first tile of the name). */
void Text_PrintWithTile(int x, int y, u16 *map, const void *text, u16 unk_30, u16 unk_32, u16 tile)
{
    Unk_027c0280 *gfx = data_027c0280;
    TextPrinter *printer = gfx->textPrinter;

    gEventSkipState = 0;
    TextPrinter_InitFull(printer, x, y, gfx->bgVram + (gfx->bgCnt[gfx->layer0Bg].charBase << 14), map, text, unk_30, unk_32, tile);
    func_02015da8(printer);
    ScriptSlot_Start(data_02158394, 0);
}
