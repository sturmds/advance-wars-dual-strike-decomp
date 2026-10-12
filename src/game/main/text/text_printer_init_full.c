#include "types.h"
#include "game/text_printer.h"

void* GfxLayer_GetCopyCallback(int arg);

void TextPrinter_InitFull(TextPrinter* printer, int x, int y, u8* tiles,
                          u16* map, const void* text, u16 unk_30, u16 unk_32,
                          u16 unk_38) {
  printer->text = text;
  printer->resumeText = 0;
  printer->tiles = tiles;
  printer->screen = map;
  printer->unk_30 = unk_30;
  printer->unk_32 = unk_32;
  printer->unk_3a = unk_38;
  printer->unk_38 = unk_38;
  printer->charSound = 0;
  printer->startX = x;
  printer->startY = y;
  printer->cursorX = x;
  printer->cursorY = y;
  printer->frameCounter = -1;
  printer->charDelay = 2;
  printer->copyCallback = GfxLayer_GetCopyCallback(0);
  printer->pixelInTile = 0;
  printer->charsDrawn = 0;
}
