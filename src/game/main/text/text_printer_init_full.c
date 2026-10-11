#include "types.h"
#include "game/text_printer.h"

void *GfxLayer_GetCopyCallback(int arg);

void TextPrinter_InitFull(TextPrinter *printer, int x, int y, u8 *tiles, u16 *map, const void *text, u16 unk_30, u16 unk_32, u16 unk_38)
{
    printer->text = text;
    printer->resumeText = 0;
    printer->tiles = tiles;
    printer->screen = map;
    printer->unk_30 = unk_30;
    printer->unk_32 = unk_32;
    printer->unk_3a = unk_38;
    printer->unk_38 = unk_38;
    printer->unk_3c = 0;
    printer->startX = x;
    printer->startY = y;
    printer->cursorX = x;
    printer->cursorY = y;
    printer->unk_3d = -1;
    printer->unk_3e = 2;
    printer->unk_40 = GfxLayer_GetCopyCallback(0);
    printer->unk_44 = 0;
    printer->unk_46 = 0;
}
