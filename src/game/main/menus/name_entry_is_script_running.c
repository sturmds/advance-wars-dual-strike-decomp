#include "types.h"
#include "game/name_entry_work.h"

extern NameEntry* gNameEntry;

int NameEntry_IsScriptRunning(void) { return gNameEntry->scriptRunning; }
