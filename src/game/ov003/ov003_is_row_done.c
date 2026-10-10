#include "types.h"
#include "game/ov003_unit.h"
#include "game/Unk_ov003_02369bb8.h"

extern Unk_ov003_02369bb8 data_ov003_02369bb8[4];

int func_ov003_0235bc50(void);

/* Returns 1 when the spawn row (not row 5) has used all its spawn records,
 * func_ov003_0235bc50 returns 0 and no data_ov003_02369bb8 entry is busy. */
int Ov003_IsRowDone(void)
{
    Unk_ov003_02369bb8 *entry;
    int i;

    if (data_ov003_02364928->row == 5) {
        return 0;
    }
    if (data_ov003_02364928->nextSpawn < data_ov003_02364928->rows[data_ov003_02364928->row - 1].spawnCount) {
        return 0;
    }
    if (func_ov003_0235bc50() != 0) {
        return 0;
    }
    entry = data_ov003_02369bb8;
    for (i = 0; i < 4; i++, entry++) {
        if (entry->busy) {
            return 0;
        }
    }
    return 1;
}
