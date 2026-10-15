#include "types.h"
#include "game/Unk_0201d320.h"
#include "game/Unk_022977a4.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/text_printer.h"

extern u8 gEventSkipState;
extern Unk_0201d320 data_022977f4[];
extern EventSlotHold data_022977f8[];

const u8* Text_Get(u32 textId);
BOOL func_02015dbc(const u8* text);
/* defined as void, but it passes on the printer TextPrinter_StartById returns */
TextPrinter* Text_PrintByIdOnLayer0(s16 x, s16 y, u16* map, u32 textId,
                                    u16 unk_30, u16 unk_38);
void func_0201c600(void);
struct EventScriptSlot;
void EventHold_ReleaseOnTextDone(struct EventScriptSlot* slot);

/* Event script command: prints the command's text id (unk_0c) on the layer 0
 * tile map at tile (7, 15), or (1, 15) in game mode 0 when
 * data_022977a4.armyColour is 5, with the current palette slot. The printer
 * runs func_0201c600 when done, and the script is held by EventHold_ReleaseOnTextDone.
 * A text with control code 20 (func_02015dbc) sets gEventSkipState to 2;
 * while gEventSkipState is 1 nothing is printed and the command returns 1,
 * so the script goes on this frame. */
s16 EventCmd_PrintText(int index) {
  Unk_0201d320* script = &data_022977f4[index];
  BOOL done;
  TextPrinter* printer;
  u16* map;
  int x; /* needed to match: an int narrowed at the call */

  if (!func_02015dbc(Text_Get(script->unk_00->arg))) {
    gEventSkipState = 2;
  }
  if (gEventSkipState != 1) {
    map = data_027c0280->layer0Map;
    if (data_027c027c->unk_48[data_027c0284]->gameMode == 0 &&
        data_022977a4.armyColour == 5) {
      x = 1;
    } else {
      x = 7;
    }
    printer = Text_PrintByIdOnLayer0(x, 15, map, script->unk_00->arg,
                                     data_022977a4.unk_00 << 12,
                                     data_022977a4.unk_04);
    printer->copyCallback = (void*)func_0201c600;
    printer->charSound = 1;
    data_022977f8[index].hold = (u32)EventHold_ReleaseOnTextDone;
    done = 0;
  } else {
    done = 1;
  }
  script->unk_00++;
  return done;
}
