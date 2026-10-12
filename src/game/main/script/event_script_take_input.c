#include "types.h"

extern u16 gEventScriptOwnsInput;

/* While set, map touch handlers do nothing and EventScript_UpdateAll
 * replaces the gKeypad state with gEventScriptKeys. */
void EventScript_TakeInput(void) { gEventScriptOwnsInput = 1; }
