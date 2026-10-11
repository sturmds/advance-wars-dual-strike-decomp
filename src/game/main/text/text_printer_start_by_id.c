#include "types.h"
#include "game/text_printer.h"

extern u8 gEventSkipState;
extern u8 data_0215837c[];

TextPrinter *ScriptSlot_Start(const void *descriptor, int arg);
void TextPrinter_InitById(TextPrinter *printer, int x, int y, u8 *tiles, u16 *map, u32 textId, u16 unk_30, u16 unk_38);

/* Starts the script data_0215837c in a free slot and sets up that slot's text
 * printer for a text id. Returns the printer. */
TextPrinter *TextPrinter_StartById(int x, int y, u8 *tiles, u16 *map, u32 textId, u16 unk_30, u16 unk_38)
{
    TextPrinter *printer;

    gEventSkipState = 0;
    printer = ScriptSlot_Start(data_0215837c, 0);
    TextPrinter_InitById(printer, x, y, tiles, map, textId, unk_30, unk_38);
    return printer;
}
