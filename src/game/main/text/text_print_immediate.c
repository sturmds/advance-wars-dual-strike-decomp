#include "types.h"
#include "game/Unk_02080440.h"
#include "game/script_slot.h"
#include "game/text_printer.h"

extern Unk_02080440* data_027c027c;

u16* TextPrinter_GetCursorEntry(TextPrinter* printer);
s8 ScriptSlot_Find(const void* script);
const void* Text_Get(u32 textId);
int TextPrinter_ReadControlCode(TextPrinter* printer, u16* entry);
void TextPrinter_WriteRun(TextPrinter* printer, u16* entry, int arg);

/* Prints the string textId at tile (x, y) of map in one call. Takes a free
 * script slot as the printer. */
void Text_PrintImmediate(int x, int y, u16* map, u32 textId, u16 unk_30,
                         int arg) {
  TextPrinter* printer;
  u16* entry;
  s16 slot;

  slot = ScriptSlot_Find(NULL);
  printer = (TextPrinter*)&data_027c027c->scriptSlots[slot];
  printer->text = Text_Get(textId);
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
