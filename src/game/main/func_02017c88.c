#include "types.h"
#include "game/play_records.h"

/* Lowers gPlayRecords.unk_04 to limit when it is higher. */
void func_02017c88(u32 limit)
{
    if (gPlayRecords.unk_04 >= limit) {
        gPlayRecords.unk_04 = limit;
    }
}
