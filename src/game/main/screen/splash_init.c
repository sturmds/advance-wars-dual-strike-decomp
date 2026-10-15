#include "types.h"
#include "game/owner_info.h"
#include "game/Unk_02088d04.h"
#include "game/splash_task.h"
#include "sdk/fs.h"

void File_UnpackTo(void* dest, const char* fileName);
void Palette_LoadFromFile(const char* fileName, int fileOffset,
                          int paletteOffset, int size);

extern const char* data_ov000_022f66b0[];
extern const char data_02169d64[];
extern const char data_02169d70[];
extern const char data_02169d78[];

/* Clears the spawner counters, loads the sprite graphics file chosen by
 * data_ov000_023503a0 into object VRAM and the palette file 0bd. */
void Splash_Init(SplashTask* task) {
  task->step = 0;
  task->lettersMade = 0;
  FS_ChangeDir(data_02169d64);
  File_UnpackTo(data_027c0280->objVram + 0x3280,
                data_ov000_022f66b0[data_ov000_023503a0]);
  FS_ChangeDir(data_02169d70);
  Palette_LoadFromFile(data_02169d78, 0, 0x260, 32);
}
