#include "types.h"
#include "game/Unk_0201d320.h"
#include "game/Unk_022977a4.h"
#include "game/Unk_02080440.h"
#include "game/text_printer.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern u8 gEventSkipState;
extern Unk_0201d320 data_022977f4[];
extern EventSlotHold data_022977f8[];
extern u8* data_02297790;
extern u16 gEventTextMap[];

const u8* Text_Get(u32 textId);
BOOL func_02015dbc(const u8* text);
TextPrinter* TextPrinter_StartById(s16 x, s16 y, u8* tiles, u16* map,
                                   u32 textId, u16 unk_30, u16 unk_38);
void EventTextMap_QueueCopy(void);
struct EventScriptSlot;
void EventHold_ReleaseOnTextDone(struct EventScriptSlot* slot);

/* Event script command: prints the command's text id (unk_0c) into the
 * event text window buffer gEventTextMap at tile (7, 1), or (1, 1) in game
 * mode 0 when data_022977a4.armyColour is 5, with the current palette slot.
 * The printer runs EventTextMap_QueueCopy (uploads the buffer) when done, and the
 * script is held by EventHold_ReleaseOnTextDone. Skipping works as in
 * EventCmd_PrintText. */
s16 EventCmd_PrintWindowText(int index) {
  Unk_0201d320* script = &data_022977f4[index];
  BOOL done;
  TextPrinter* printer;
  int x; /* needed to match: an int narrowed at the call */

  if (!func_02015dbc(Text_Get(script->unk_00->arg))) {
    gEventSkipState = 2;
  }
  if (gEventSkipState != 1) {
    if (data_027c027c->unk_48[data_027c0284]->gameMode == 0 &&
        data_022977a4.armyColour == 5) {
      x = 1;
    } else {
      x = 7;
    }
    printer = TextPrinter_StartById(
        x, 1, data_02297790, gEventTextMap, script->unk_00->arg,
        data_022977a4.unk_00 << 12, data_022977a4.unk_04);
    printer->copyCallback = (void*)EventTextMap_QueueCopy;
    printer->charSound = 1;
    data_022977f8[index].hold = (u32)EventHold_ReleaseOnTextDone;
    done = 0;
  } else {
    done = 1;
  }
  script->unk_00++;
  return done;
}
