#include "types.h"
#include "game/name_entry_work.h"

extern NameEntry *gNameEntry;

/* Last command of the name entry scripts: lets NameEntry_Update go on. */
void NameEntry_EndScript(void)
{
    gNameEntry->scriptRunning = 0;
}
