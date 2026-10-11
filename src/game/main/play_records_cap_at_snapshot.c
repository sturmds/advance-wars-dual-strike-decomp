#include "types.h"
#include "game/play_records_snapshot.h"

void func_02017ca0(int limit);
void func_02017c88(int limit);

/* Lowers the first two counters of gPlayRecords to the values in
 * data_022a7fc8 when they are higher. */
void PlayRecords_CapAtSnapshot(void)
{
    func_02017ca0(data_022a7fc8.unk_00);
    func_02017c88(data_022a7fc8.unk_04);
}
