#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"

void FrameCallback_Remove(void (*callback)(void));
void func_01ff9c28(void (*callback)(void));
void MainLoop_SetFrameCallback(void (*callback)(void));
void func_020c0704(void);
void func_020d1be0(void);
void WeatherFx_UpdateFirstScreen(void);
void MainLoop_DefaultVBlank(void);
void MainLoop_TaskFrame(void);

/* Name is a guess. Drops the per-frame callback of the current map screen
 * (WeatherFx_UpdateFirstScreen for the main map, func_020d1be0 for the second) and puts
 * the main loop's two callbacks back to MainLoop_DefaultVBlank and MainLoop_TaskFrame. */
void Map_ResetFrameCallbacks(void) {
  if ((void*)data_027c027c == data_027c0288.unk_00) {
    FrameCallback_Remove(WeatherFx_UpdateFirstScreen);
  } else {
    FrameCallback_Remove(func_020d1be0);
  }
  func_01ff9c28(MainLoop_DefaultVBlank);
  MainLoop_SetFrameCallback(MainLoop_TaskFrame);
  func_020c0704();
}
