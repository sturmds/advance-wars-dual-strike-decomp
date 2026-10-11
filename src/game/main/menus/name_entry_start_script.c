#include "types.h"
#include "game/name_entry_work.h"

extern NameEntry *gNameEntry;

void *EventScript_Start(const void *script);

void NameEntry_StartScript(const void *script)
{
    gNameEntry->scriptRunning = 1;
    EventScript_Start(script);
}
