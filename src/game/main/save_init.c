#include "types.h"

extern void (*data_02297f24)(void);
extern u8 *data_02297f28;
extern int (*data_02297f2c)(u8 *dest);

void Save_DetectBackupMemory(void);
void Save_ScanSlots(int noFormat);

/* Sets up the save system: stores the pack function (writes the game's save data to a buffer
 * and returns its size), the reset function (Save_Format calls it to start a fresh save) and
 * the buffer, then detects the backup memory and reads all slots. */
void Save_Init(int (*pack)(u8 *dest), void (*reset)(void), u8 *buffer)
{
    data_02297f2c = pack;
    data_02297f24 = reset;
    data_02297f28 = buffer;
    Save_DetectBackupMemory();
    Save_ScanSlots(0);
}
