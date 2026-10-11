#include "types.h"
#include "game/save_data.h"

extern u8 data_0229b050;

int func_0200c150(int offset, SaveBuffer *buffer, int size);

/* Compares size bytes of buffer with save slot slot in the backup memory.
 * Without the backup memory (data_0229b050 not 1) it returns buffer, which
 * Save_WriteRecord counts as a pass. */
int Save_VerifyBackup(u16 slot, SaveBuffer *buffer, int size)
{
    int result = (int)buffer;

    if (data_0229b050 == 1) {
        result = func_0200c150(slot * 0x3000, buffer, size);
    }
    return result;
}
