#include "types.h"
#include "game/save_data.h"

extern u8 gHasBackupMemory;

int func_0200c1c0(int offset, void* buffer, int size);
int func_0200c230(int offset, SaveBuffer* buffer, int size);
void Link_Stop(void);
void SpriteTask_DestroyAllFirstScreen(void);
void func_0201fb3c(void);

/* Writes size bytes of buffer to save slot slot in the backup memory. Before
 * the write it reads one byte back from offset 0; if that fails, it stops
 * the link and the first-screen sprites and shows the error screen. Returns
 * 1 without the backup memory (gHasBackupMemory not 1). */
u16 Save_WriteToBackup(u16 slot, SaveBuffer* buffer, int size) {
  u8 probe;

  if (gHasBackupMemory == 1) {
    if (!func_0200c1c0(0, &probe, 1)) {
      Link_Stop();
      SpriteTask_DestroyAllFirstScreen();
      func_0201fb3c();
    }
    return func_0200c230(slot * 0x3000, buffer, size);
  }
  return 1;
}
