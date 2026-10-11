#include "types.h"
#include "game/save_data.h"

extern u8 gHasBackupMemory;

int func_0200c1c0(int offset, SaveBuffer *buffer, int size);
void Link_Stop(void);
void SpriteTask_DestroyAllFirstScreen(void);
void func_0201fb3c(void);

/* Reads size bytes of save slot slot from the backup memory into buffer. When
 * the read fails, it stops the link and the first-screen sprites and shows
 * the error screen. Does nothing unless gHasBackupMemory is 1. The second
 * argument is not used. */
void Save_ReadFromBackup(int slot, int unused, SaveBuffer *buffer, int size)
{
    if (gHasBackupMemory != 1) {
        return;
    }
    if (func_0200c1c0(slot * 0x3000, buffer, size)) {
        return;
    }
    Link_Stop();
    SpriteTask_DestroyAllFirstScreen();
    func_0201fb3c();
}
