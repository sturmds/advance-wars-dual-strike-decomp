#include "types.h"
#include "game/Unk_02088d04.h"
#include "game/text_printer.h"

extern Unk_027c0280* data_027c0280;

/* Copies the text printer in data_027c0280 into printer, field by field. */
void TextPrinter_CopyFromShared(TextPrinter* printer) {
  TextPrinter* src = data_027c0280->textPrinter;

  printer->text = src->text;
  printer->resumeText = src->resumeText;
  printer->tiles = src->tiles;
  printer->screen = src->screen;
  printer->unk_30 = src->unk_30;
  printer->unk_32 = src->unk_32;
  printer->unk_3a = src->unk_38;
  printer->unk_38 = src->unk_38;
  printer->charSound = src->charSound;
  printer->startX = src->startX;
  printer->startY = src->startY;
  printer->cursorX = src->cursorX;
  printer->cursorY = src->cursorY;
  printer->frameCounter = src->frameCounter;
  printer->charDelay = src->charDelay;
  printer->copyCallback = src->copyCallback;
  printer->pixelInTile = src->pixelInTile;
  printer->charsDrawn = src->charsDrawn;
}
