#include "types.h"
#include "game/event_script.h"

BOOL Text_IsPrinting(void);

/* Hold function of the text printing commands: clears the slot's hold once
 * the text has been printed. */
void EventHold_ReleaseOnTextDone(EventScriptSlot* slot) {
  if (Text_IsPrinting() == FALSE) {
    slot->hold = 0;
  }
}
