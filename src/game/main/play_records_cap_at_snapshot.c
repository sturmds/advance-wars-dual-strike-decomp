#include "types.h"
#include "game/play_records_snapshot.h"

void func_02017ca0(int limit);
void func_02017c88(int limit);

/* Lowers the first two counters of gPlayRecords to the values in
 * gPlayRecordsSnapshot when they are higher. */
void PlayRecords_CapAtSnapshot(void)
{
    func_02017ca0(gPlayRecordsSnapshot.points);
    func_02017c88(gPlayRecordsSnapshot.unk_04);
}
