#include "types.h"
#include "game/ov003_gfx.h"
#include "game/Unk_02088d04.h"

void Mem_Free(void* block);
void Wallpaper_DestroyTasks(void);
void Ov003_StartExitTask(void);
void func_ov003_02357178(void);
void func_ov003_0235ca24(void);
void Ov003_DestroyTasks(void);
void func_ov003_02353bac(void);

extern Unk_027c0280* data_027c0280;
extern void* data_ov003_02365b10;

/* Undoes Ov003_Init: tears down the overlay's tasks and frees its two buffers. */
void Ov003_Exit(void) {
  GFX_DISPCNT(data_027c0280)->objWin = 0;
  func_ov003_02357178();
  func_ov003_0235ca24();
  Ov003_DestroyTasks();
  func_ov003_02353bac();
  Wallpaper_DestroyTasks();
  Mem_Free(data_ov003_02365b10);
  Mem_Free(data_ov003_02365b0c);
  Ov003_StartExitTask();
}
