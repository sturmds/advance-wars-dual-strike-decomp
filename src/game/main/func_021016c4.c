#include "types.h"
#include "game/Unk_022a7ea8.h"

extern Unk_022a7ea8 *data_022a7ea8;

void *EventScript_Start(const void *script);

void func_021016c4(const void *script)
{
    data_022a7ea8->unk_118 = 1;
    EventScript_Start(script);
}
