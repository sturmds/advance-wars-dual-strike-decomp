#include "types.h"

extern void (*gSaveResetFunc)(void);
extern u8* gSaveBuffer;
extern int (*gSavePackFunc)(u8* dest);

void Save_DetectBackupMemory(void);
void Save_ScanSlots(int noFormat);

/* Sets up the save system: stores the pack function (writes the game's save data to a buffer
 * and returns its size), the reset function (Save_Format calls it to start a fresh save) and
 * the buffer, then detects the backup memory and reads all slots. */
void Save_Init(int (*pack)(u8* dest), void (*reset)(void), u8* buffer) {
  gSavePackFunc = pack;
  gSaveResetFunc = reset;
  gSaveBuffer = buffer;
  Save_DetectBackupMemory();
  Save_ScanSlots(0);
}
