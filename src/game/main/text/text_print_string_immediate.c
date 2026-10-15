#include "types.h"
#include "game/Unk_02080440.h"
#include "game/script_slot.h"
#include "game/text_printer.h"

u16* TextPrinter_GetCursorEntry(TextPrinter* printer);
s8 ScriptSlot_Find(const void* script);
int TextPrinter_ReadControlCode(TextPrinter* printer, u16* entry);
void TextPrinter_WriteRun(TextPrinter* printer, u16* entry, int arg);

/* Same as Text_PrintImmediate, but takes the string itself instead of a text id. */
void Text_PrintStringImmediate(int x, int y, u16* map, const void* text,
                               u16 unk_30, int arg) {
  TextPrinter* printer;
  u16* entry;
  s16 slot;

  slot = ScriptSlot_Find(NULL);
  printer = (TextPrinter*)&data_027c027c->scriptSlots[slot];
  printer->text = text;
  printer->resumeText = NULL;
  printer->screen = map;
  printer->unk_30 = unk_30;
  printer->unk_32 = 0;
  printer->startX = x;
  printer->cursorX = x;
  printer->cursorY = y;
  while (1) {
    entry = TextPrinter_GetCursorEntry(printer);
    switch (TextPrinter_ReadControlCode(printer, entry)) {
      case 0:
        return;
      case 1:
        TextPrinter_WriteRun(printer, entry, arg);
        break;
      case 2:
      case 3:
        break;
    }
  }
}
