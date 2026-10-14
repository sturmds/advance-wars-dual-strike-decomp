#include "types.h"
#include "game/Unk_0201d320.h"
#include "game/keypad.h"

extern Unk_0201d320 data_022977f4[];

void EventScript_TakeInput(void);

/* Event script command: waits until one of the keys in the record's mask is
 * down (gKeypad->nextPressed). Then it calls the record's function, moves to the
 * next record and ends the slot's run for this frame. */
BOOL EventCmd_WaitForKeys(s16 index) {
  Unk_0201d320* script = &data_022977f4[index];
  const EventScriptCommand* entry = script->unk_00;
  void (*run)() = (void (*)())entry->u.ptr; /* called with no argument */

  if (gKeypad->nextPressed & entry->arg) {
    run();
  } else {
    return 0;
  }
  EventScript_TakeInput();
  script->unk_00++;
  return 0;
}
