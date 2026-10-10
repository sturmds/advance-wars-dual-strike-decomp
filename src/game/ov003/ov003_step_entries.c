#include "types.h"
#include "game/ov003_unit.h"
#include "game/Unk_ov003_02369bb8.h"

extern Unk_ov003_02369bb8 data_ov003_02369bb8[4];

void func_ov003_0235e2c4(Unk_ov003_02369bb8 *entry);

/* Steps every busy entry unless Ov003State.unk_1db_7 is set. */
void Ov003_StepEntries(void)
{
    Unk_ov003_02369bb8 *entry = data_ov003_02369bb8;
    int i;

    if (data_ov003_02364928->unk_1db_7) {
        return;
    }
    for (i = 0; i < 4; i++, entry++) {
        if (entry->busy) {
            func_ov003_0235e2c4(entry);
        }
    }
}
