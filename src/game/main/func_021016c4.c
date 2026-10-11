#include "types.h"
#include "game/name_entry_work.h"

extern NameEntry *gNameEntry;

void *EventScript_Start(const void *script);

void func_021016c4(const void *script)
{
    gNameEntry->unk_118 = 1;
    EventScript_Start(script);
}
