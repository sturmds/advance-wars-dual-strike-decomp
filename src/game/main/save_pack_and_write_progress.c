#include "types.h"
#include "game/save_progress.h"

int Save_PackProgress(SaveProgressBlocks *record);
void Save_WriteRecord(int recordId, void *buffer, int size);
void func_02019920(void);
void func_020d6fcc(void);

/* Packs the live progress data into data_02291774 and writes it as save
 * record 0. */
void func_02018f48(void)
{
    func_02019920();
    Save_WriteRecord(0, &data_02291774, Save_PackProgress((SaveProgressBlocks *)&data_02291774));
    func_020d6fcc();
}
